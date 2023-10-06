#include "simulation.hpp"

using namespace cgp;

// Convert a density value to a pressure
float density_to_pressure(float rho, float rho0, float stiffness) {
  return stiffness * (rho - rho0);
}

float W_laplacian_viscosity(vec3 const &p_i, vec3 const &p_j, float h) {
  // To do ...
  //  Fill it with laplacian of W_viscosity
  float const r = norm(p_i - p_j);
  return (45.0f / (Pi * std::pow(h, 6.0f))) * (h - r);
}

vec3 W_gradient_pressure(vec3 const &p_i, vec3 const &p_j, float h) {
    // To do ...
    //  Fill it with gradient of W_spiky
    float const r = norm(p_i - p_j);
    return -(45.0f / (Pi * std::pow(h, 6.0f))) * std::pow((h - r), 2.0f) * ((p_i - p_j) / r);
}

float W_density(vec3 const &p_i, const vec3 &p_j, float h) {
    float const r = norm(p_i - p_j);
    assert_cgp_no_msg(r <= h);
    return 315.0 / (64.0 * 3.14159f * std::pow(h, 9)) * std::pow(h * h - r * r, 3.0f);
}


void update_density(std::vector<particle_element> &particles, float h, float m) {
    // To do: Compute the density value (particles[i].rho) at each particle position
    int const N = particles.size();
    for (int i = 0; i < N; ++i) {
        particles[i].rho = 0.0f;
        for (int j = 0; j < N; ++j) {
            if (norm(particles[i].p - particles[j].p) <= h)
                particles[i].rho += W_density(particles[i].p, particles[j].p, h) * m;
        }
    }
}

// Convert the particle density to pressure
void update_pressure(std::vector<particle_element> &particles, float rho0, float stiffness) {
    const int N = particles.size();
    for (int i = 0; i < N; ++i)
        particles[i].pressure = density_to_pressure(particles[i].rho, rho0, stiffness);
}

// Compute the forces and update the acceleration of the particles
void update_force(std::vector<particle_element> &particles, float h, float m, float nu) {
    // gravity
    const int N = particles.size();
    for (int i = 0; i < N; ++i) {
        vec3 F_pressure = vec3{};
        vec3 F_viscosity = vec3{};
        particles[i].f = m * vec3{0, 0, -9.81f * 4.0f};
        for (int j = 0; j < N; j++) {
            if (i != j && norm(particles[i].p - particles[j].p) <= h) {
                F_pressure += m * (particles[i].pressure + particles[j].pressure) / (2 * particles[j].rho) * W_gradient_pressure(particles[i].p, particles[j].p, h);
                F_viscosity += m * (particles[j].v - particles[i].v) / particles[j].rho * W_laplacian_viscosity(particles[i].p, particles[j].p, h);
            }
        }
        particles[i].f += (F_pressure * (-m / particles[i].rho) + m * nu * (F_viscosity));
    }
}

void simulate(float dt, std::vector<particle_element> &particles, sph_parameters_structure const &sph_parameters, vec3 const &b_min, vec3 const &b_max, vec3 const &penguin_speed) {

    // Update values
    update_density(particles, sph_parameters.h, sph_parameters.m);                   // First compute updated density
    update_pressure(particles, sph_parameters.rho0, sph_parameters.stiffness);       // Compute associated pressure
    update_force(particles, sph_parameters.h, sph_parameters.m, sph_parameters.nu);  // Update forces

    // Numerical integration
    float const damping = 0.005f;
    int const N = particles.size();
    float const m = sph_parameters.m;
    for (int k = 0; k < N; ++k) {
        vec3 &p = particles[k].p;
        vec3 &v = particles[k].v;
        vec3 &f = particles[k].f;

        v = (1 - damping) * v + dt * f / m;
        p = p + dt * v;
    }

    // Collision
    float const &r = sph_parameters.h;
    float const epsilon = 1e-3f;

    vec3 pts_arr[6] = {b_min, {b_min.x, b_min.y, b_max.z}, {b_min.x, b_max.y, b_max.z}, b_max, {b_max.x, b_max.y, b_min.z}, {b_max.x, b_min.y, b_min.z}};

    for (int k = 0; k < N; ++k) {
        vec3 &p = particles[k].p;
        vec3 &v = particles[k].v;

        for (int i = 0; i < 6; i++) {
            vec3 &pts1 = pts_arr[i];
            vec3 &pts2 = pts_arr[(i + 1) % 6];
            vec3 &pts3 = pts_arr[(i + 2) % 6];
            vec3 diag = p - pts1;
            vec3 u = normalize(pts2 - pts1);
            vec3 w = normalize(pts3 - pts1);
            vec3 u_offset = u * dot(u, diag) * norm(diag);
            vec3 w_offset = w * dot(w, diag) * norm(diag);
            vec3 point_on_plane = pts1 + u_offset + w_offset;
            if (dot(u, point_on_plane) > 0 && dot(w, point_on_plane) > 0 && norm(pts1 - (pts1 + u_offset)) < norm(pts1 - pts2) && norm(pts1 - (pts1 + w_offset)) < norm(pts1 - pts3) && norm(point_on_plane - p) <= r)
                v = (normalize(v) + 2 * normalize(p - point_on_plane)) * norm(v) + 0.01f * penguin_speed;
        }

        float distance_from_center = norm(vec2{-25,0} - p.xy());
        vec2 to_center = normalize(vec2{-25,0} - p.xy());
        if (distance_from_center > 4)
        {
            vec2 offset = (distance_from_center - 4 + epsilon * rand_interval()) * normalize(to_center);
            p.x += offset.x;
            p.y += offset.y;
            vec2 new_speed = (normalize(v.xy()) + 2 * normalize(to_center)) * norm(v.xy()) * 0.5f;
            v.x = new_speed.x;
            v.y = new_speed.y;
        }
        // small perturbation to avoid alignment
        /*
        if (p.y < -4) {
            p.y = -4 + epsilon * rand_interval();
            v.y *= -0.5f;
        }
        if (p.y > 4) {
            p.y = 4 - epsilon * rand_interval();
            v.y *= -0.5f;
        }

        if (p.x < -29) {
            p.x = -29 + epsilon * rand_interval();
            v.x *= -0.5f;
        }
        if (p.x > -21) {
            p.x = -21 - epsilon * rand_interval();
            v.x *= -0.5f;
        } */

        if (p.z < -1) {
            p.z = -1 + epsilon * rand_interval();
            v.z *= -0.5f;
        }

    }

}
