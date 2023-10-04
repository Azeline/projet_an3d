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
  floor.initialize_data_on_gpu(mesh_load_file_obj("../assets/banquise8.obj"));
  floor.texture.load_and_initialize_texture_2d_on_gpu("../assets/texture_banquise.png", GL_REPEAT, GL_REPEAT);
  floor.material.phong.specular = 0.1f;
  hierarchy.add(floor, "Floor");

  create_penguin(hierarchy);
  penguin_ready(hierarchy);
}

void scene_structure::display_frame() {
  // Set the light to the current position of the camera
  // environment.light = camera_control.camera_model.position();
  if (gui.display_frame) draw(global_frame, environment);

  // Update the current time
  timer.update();

  // This function must be called before the drawing in order to propagate the
  // deformations through the hierarchy
  hierarchy.update_local_to_global_coordinates();

  // Draw the hierarchy as a single mesh
  draw(hierarchy, environment);
  if (gui.display_wireframe) draw_wireframe(hierarchy, environment);
}

void scene_structure::display_gui() {
  ImGui::Checkbox("Frame", &gui.display_frame);
  ImGui::Checkbox("Wireframe", &gui.display_wireframe);
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
