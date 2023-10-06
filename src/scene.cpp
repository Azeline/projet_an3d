#include "scene.hpp"

using namespace cgp;

void scene_structure::initialize() {
  // Basic set-up
  // ***************************************** //
  camera_control.initialize(inputs,
                            window);  // Give access to the inputs and window
                                      // global state to the camera controler
  camera_control.set_rotation_axis_z();
  camera_control.look_at({2.0f, -2.0f, 1.0f}, {0, 0, 0});

  global_frame.initialize_data_on_gpu(mesh_primitive_frame());

  mesh_drawable floor;
  floor.initialize_data_on_gpu(mesh_load_file_obj("../assets/banquise.obj"));
  floor.texture.load_and_initialize_texture_2d_on_gpu("../assets/snow.jpg", GL_REPEAT, GL_REPEAT);
  floor.material.phong.specular = 0.1f;
  hierarchy.add(floor, "Floor");

  create_penguin(hierarchy, penguin_struct);
  penguin_ready(hierarchy, penguin_struct);
  // Sphere init
  // ***************************************** //
  initialize_sph();
  sphere_particle.initialize_data_on_gpu(mesh_primitive_sphere(1.0,{0,0,0},10,10));
  sphere_particle.model.scaling = 0.01f;
  curve_visual.color = { 1,0,0 };
  curve_visual.initialize_data_on_gpu(curve_primitive_circle());

  // Initialization for the Field function
  // ***************************************** //
  field_function.particles = &particles;

  // Initialization for the Implicit Surface
  // ***************************************** //
  implicit_surface.set_domain(gui.domain.samples, gui.domain.length);
  implicit_surface.update_field(field_function, gui.isovalue);
}

void scene_structure::initialize_sph()
{
    // Initial particle spacing (relative to h)
    float const c = 0.5f;
    float const h = sph_parameters.h;
    float xOffset = sph_parameters.spawnLocation.x;


    // Fill a square with particles
    particles.clear();
    for (float x = xOffset + h; x < xOffset + 1.0f - h; x = x + c * h)
    {
        for (float y = -1.0f + h; y < 1.0f - h; y = y + c * h)
        {
            for (float z = -1.0f + h; z < 1.0f - h; z = z + c * h)
            {
                particle_element particle;
                particle.p = {x + h / 8.0 * rand_interval(), y + h / 8.0 * rand_interval(), z + h / 8.0 * rand_interval()}; // a zero value in z position will lead to a 2D simulation
                particles.push_back(particle);
            }
        }
    }
    printf("nb particles : %li\n", particles.size());
}

void scene_structure::display_frame() {
  // Set the light to the current position of the camera
  environment.light = camera_control.camera_model.position();
  if (gui.display.frame) draw(global_frame, environment);

  // Update the current time
  timer.update();
  float const dt = 0.005f * timer.scale;
  simulate_penguin(hierarchy, penguin_struct, dt);
  hierarchy.update_local_to_global_coordinates();
  implicit_surface.time_update(gui, field_function);

  // Draw the hierarchy as a single mesh
  draw(hierarchy, environment);
  if (gui.display.wireframe) {
      draw_wireframe(hierarchy, environment);
      draw_wireframe(implicit_surface.drawable_param.shape, environment);
  }

  // ***************************************** //
    simulate(dt, particles, sph_parameters);


    if (gui.display.particles) {
        for (int k = 0; k < particles.size(); ++k) {
            vec3 const& p = particles[k].p;
            sphere_particle.model.translation = p;
            draw(sphere_particle, environment);
        }
    }

    if (gui.display.radius) {
        curve_visual.model.scaling = sph_parameters.h;
        for (int k = 0; k < particles.size(); k += 10) {
            curve_visual.model.translation = particles[k].p;
            draw(curve_visual, environment);
        }
    }

    if (gui.display.surface)    // Display the implicit surface
        draw(implicit_surface.drawable_param.shape, environment);

    if (gui.display.domain)    // Display the boundary of the domain
        draw(implicit_surface.drawable_param.domain_box, environment);

}

void scene_structure::display_gui() {

  ImGui::SliderFloat("Timer scale", &timer.scale, 0.01f, 4.0f, "%0.2f");

  bool const restart = ImGui::Button("Restart");
  if (restart) {
      initialize_sph();
      penguin_struct.v = {};
      penguin_struct.center = {0,0,1.7f};
      hierarchy["Body1"].transform_local.translation = penguin_struct.center;
      penguin_struct.bounding_min = { INFINITY,INFINITY,INFINITY };
      penguin_struct.bounding_max = { -INFINITY,-INFINITY,-INFINITY };
      get_bounding_box(hierarchy, penguin_struct);
  }

    implicit_surface.gui_update(gui, field_function, penguin_struct);
}

void scene_structure::mouse_move_event() {
  if (!inputs.keyboard.shift)
    camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event() {
  camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event() {
  camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame() {
  camera_control.idle_frame(environment.camera_view);
}
