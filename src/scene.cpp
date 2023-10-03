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

  mesh_drawable floor1;
  mesh_drawable floor2;
  mesh_drawable floor3;
  
  auto size_ice1 = 20;
  auto size_ice2 = 4;
  auto size_ice3 = 8;
  auto height = -1;
  floor1.initialize_data_on_gpu(mesh_primitive_cubic_grid({ -size_ice1,-size_ice1,0 }, { -size_ice1,size_ice1,0 }, { size_ice1,size_ice1,0 }, { size_ice1,-size_ice1,0 }, { -size_ice1,-size_ice1,height }, { -size_ice1,size_ice1,height }, { size_ice1,size_ice1,height }, { size_ice1,-size_ice1,height }));
  floor2.initialize_data_on_gpu(mesh_primitive_cubic_grid({ -9,-size_ice2/2.f,0 }, { -9,size_ice2/2.f,0 }, { 9,size_ice2/2.f,0 }, { 9,-size_ice2/2.f,0 }, { -9,-size_ice2/2.f,height }, { -9,size_ice2/2.f,height }, { 9,size_ice2/2.f,height }, { 9,-size_ice2/2.f,height }));
  floor3.initialize_data_on_gpu(mesh_primitive_cubic_grid({ -size_ice1,-size_ice3,0 }, { -size_ice1,size_ice3,0 }, { size_ice1,size_ice3,0 }, { size_ice1,-size_ice3,0 }, { -size_ice1,-size_ice3,height }, { -size_ice1,size_ice3,height }, { size_ice1,size_ice3,height }, { size_ice1,-size_ice3,height }));
  
  auto change2ice = { &floor1, &floor2, &floor3 };
  for (auto m : change2ice) {
  	m->material.color = { 191.f/255.f, 251.f/255.f, 255.f/255.f };
    m->material.phong.specular = 0.1f;
    // m->texture.load_and_initialize_texture_2d_on_gpu("../assets/ice.jpg", 0x2901);
  }

  hierarchy.add(floor1, "Floor1");
  hierarchy.add(floor2, "Floor2.1", "Floor1", { -11,-size_ice1-size_ice2/2.f,0 });
  hierarchy.add(floor2, "Floor2.2", "Floor1", { 11,-size_ice1-size_ice2/2.f,0 });
  hierarchy.add(floor3, "Floor3", "Floor1", { 0,-size_ice1-size_ice2-size_ice3,0 });

  create_penguin(hierarchy);
//   create_penguin_cartoon(hierarchy);
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
