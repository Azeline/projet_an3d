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
        particles[i].f = m * vec3{0, 0, -9.81f};
        for (int j = 0; j < N; j++) {
            if (i != j && norm(particles[i].p - particles[j].p) <= h) {
                F_pressure += m * (particles[i].pressure + particles[j].pressure) / (2 * particles[j].rho) * W_gradient_pressure(particles[i].p, particles[j].p, h);
                F_viscosity += m * (particles[j].v - particles[i].v) / particles[j].rho * W_laplacian_viscosity(particles[i].p, particles[j].p, h);
            }
        }
        particles[i].f += (F_pressure * (-m / particles[i].rho) + m * nu* (F_viscosity));
    }
}

void simulate(float dt, std::vector<particle_element> &particles, sph_parameters_structure const &sph_parameters) {

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
    float const epsilon = 1e-3f;
    for (int k = 0; k < N; ++k) {
        vec3 &p = particles[k].p;
        vec3 &v = particles[k].v;

        // small perturbation to avoid alignment
        if (p.y < -4) {
            p.y = -1 + epsilon * rand_interval();
            v.y *= -0.5f;
        }
        if (p.y > 4) {
            p.y = 1 - epsilon * rand_interval();
            v.y *= -0.5f;
        }

        if (p.x < -29) {
            p.x = -1 + epsilon * rand_interval();
            v.x *= -0.5f;
        }
        if (p.x > 21) {
            p.x = 1 - epsilon * rand_interval();
            v.x *= -0.5f;
        }

        if (p.z < -1) {
            p.z = -1 + epsilon * rand_interval();
            v.z *= -0.5f;
        }
    }

}
