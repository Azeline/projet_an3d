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

  create_penguin(hierarchy);
//   create_penguin_cartoon(hierarchy);
}

void scene_structure::display_frame() {
  // Set the light to the current position of the camera
  environment.light = camera_control.camera_model.position();
  if (gui.display_frame) draw(global_frame, environment);

  // Update the current time
  timer.update();

  // Apply transformation to some elements of the hierarchy
  // hierarchy["Cube base"].transform_local.rotation =
  // rotation_transform::from_axis_angle({ 1,0,0 }, sin(timer.t));
  // hierarchy["Cylinder1"].transform_local.rotation =
  // rotation_transform::from_axis_angle({ 0,0,1 }, timer.t);
  // hierarchy["Cube1"].transform_local.rotation =
  // rotation_transform::from_axis_angle({ 1,0,0 }, -3 * timer.t);
  // hierarchy["Cylinder1 son"].transform_local.rotation =
  // rotation_transform::from_axis_angle({ 0,0,1 }, -10 * timer.t);

  // hierarchy["Wing Right1"].transform_local.rotation =
  // rotation_transform::from_axis_angle({1,0,0},sin(6 * timer.t));
  // hierarchy["Wing Right2"].transform_local.rotation =
  // rotation_transform::from_axis_angle({1,0,0},sin(6 * timer.t));
  // hierarchy["Wing Left1"].transform_local.rotation =
  // rotation_transform::from_axis_angle({1,0,0},sin(-6 * timer.t));
  // hierarchy["Wing Left2"].transform_local.rotation =
  // rotation_transform::from_axis_angle({1,0,0},sin(-6 * timer.t));
  // hierarchy["head1"].transform_local.rotation =
  // rotation_transform::from_axis_angle({0,1,0},sin(2*timer.t)/4);

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
