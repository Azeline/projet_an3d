#include "penguin.hpp"

using namespace cgp;

void create_penguin(cgp::hierarchy_mesh_drawable &hierarchy) {
  mesh_drawable body1;
  mesh_drawable body2;
  mesh_drawable head1;
  mesh_drawable head2;
  mesh_drawable eye1;
  mesh_drawable eye2;
  mesh_drawable bec;
  mesh_drawable wing_right;
  mesh_drawable wing_left;
  mesh_drawable queue;
  mesh_drawable leg_right;
  mesh_drawable leg_left;
  mesh_drawable foot_right;
  mesh_drawable foot_left;

  body1.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.4f,0.45f,0.75f }));
  body1.model.rotation = rotation_transform::from_axis_angle({0,0,1},-90*Pi/180) * rotation_transform::from_axis_angle({0,1,0},40*Pi/180);
  body2.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.28f,0.32f,0.25f }));
  body2.model.rotation = rotation_transform::from_axis_angle({1,0,0},-10*Pi/180);
  head1.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.25f,0.25f,0.33f }));
  head1.model.rotation = rotation_transform::from_axis_angle({0,0,1},-90*Pi/180) * rotation_transform::from_axis_angle({0,1,0},10*Pi/180);
  head2.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.17f,0.19f,0.26f }));
  head2.model.rotation = rotation_transform::from_axis_angle({0,0,1},-90*Pi/180) * rotation_transform::from_axis_angle({0,1,0},90*Pi/180);
  eye1.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.01f,0.035f,0.03f }));
  eye1.model.rotation = rotation_transform::from_axis_angle({0,1,0},-20*Pi/180) * rotation_transform::from_axis_angle({0,0,1},-15*Pi/180);
  eye2.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.01f,0.035f,0.03f }));
  eye2.model.rotation = rotation_transform::from_axis_angle({0,1,0},20*Pi/180) * rotation_transform::from_axis_angle({0,0,1},15*Pi/180);
  bec.initialize_data_on_gpu(mesh_primitive_cubic_grid({ -0.05f,-0.35f,-0.03f }, { 0,-0.3f,0.02f }, { 0.05f,-0.35f,-0.03f }, { 0,-0.4f,-0.08f }, { -0.1f,0,0 }, { 0,0,0.1f }, { 0.1f,0,0 }, { 0,0,-0.1f }));
  wing_right.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.01f,0.1f,0.4f }));
  wing_right.model.rotation = rotation_transform::from_axis_angle({0,1,0},-15*Pi/180) * rotation_transform::from_axis_angle({1,0,0},30*Pi/180);
  wing_left.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.01f,0.1f,0.4f }));
  wing_left.model.rotation = rotation_transform::from_axis_angle({0,1,0},15*Pi/180) * rotation_transform::from_axis_angle({1,0,0},30*Pi/180);
  queue.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.25f,0.2f,0.4f }));
  queue.model.rotation = rotation_transform::from_axis_angle({1,0,0},70*Pi/180);
  leg_right.initialize_data_on_gpu(mesh_primitive_cylinder(0.07f, { 0,0,0 }, { 0,0,0.2f }, 10, 20, true));
  leg_left.initialize_data_on_gpu(mesh_primitive_cylinder(0.07f, { 0,0,0 }, { 0,0,0.2f }, 10, 20, true));
  foot_right.initialize_data_on_gpu(mesh_primitive_cubic_grid({ 0,0,0.06f }, { 0.14f,0,0.06f }, { 0.24f,-0.3f,0.03f }, { -0.08f,-0.3f,0.03f }, { 0,0,0 }, { 0.14f,0,0 }, { 0.24f,-0.3f,0 }, { -0.08f,-0.3f,0 }));
  foot_left.initialize_data_on_gpu(mesh_primitive_cubic_grid({ 0,0,0.06f }, { 0.14f,0,0.06f }, { 0.22f,-0.3f,0.03f }, { -0.1f,-0.3f,0.03f }, { 0,0,0 }, { 0.14f,0,0 }, { 0.22f,-0.3f,0 }, { -0.1f,-0.3f,0 }));

  auto change2grey2 = { &body2, &wing_right, &wing_left, &queue, &leg_right, &leg_left, &foot_right, &foot_left };
  for (auto m : change2grey2)
  	m->material.color = { 0.3f,0.3f,0.3f };
  auto change2black = { &eye1, &eye2, &bec };
  for (auto m : change2black)
  	m->material.color = { 0.2f,0.2f,0.2f };
  auto changeNoSpecular2 = { &body1, &body2, &head1, &head2, &bec, &wing_right, &wing_left, &queue, &leg_right, &leg_left, &foot_right, &foot_left };
  for (auto m : changeNoSpecular2)
    m->material.phong.specular = 0;

  body1.texture.load_and_initialize_texture_2d_on_gpu("../assets/pinguin2.png");
  head1.texture.load_and_initialize_texture_2d_on_gpu("../assets/pinguin2.png");
  head2.texture.load_and_initialize_texture_2d_on_gpu("../assets/pinguin_head.png");

  hierarchy.add(body1, "Body1");
  hierarchy.add(body2, "Body2", "Body1", { 0,-0.25f,0.5f });
  hierarchy.add(head1, "Head1", "Body2", { 0,0,0.12f });
  hierarchy.add(head2, "Head2", "Head1", { 0,-0.08f,0.16f });
  hierarchy.add(eye1, "Eye1", "Head2", {0.165f,-0.1f,0.05f});
  hierarchy.add(eye2, "Eye2", "Head2", {-0.165f,-0.1f,0.05f});
  hierarchy.add(bec, "Bec", "Head2", { 0,-0.1f,0 });
  hierarchy.add(wing_right, "Wing Right", "Body1", { 0.46f,0,0 });
  hierarchy.add(wing_left, "Wing Left", "Body1", { -0.46f,0,0 });
  hierarchy.add(queue, "Queue", "Body1", { 0,0.25f,-0.4f });
  hierarchy.add(leg_right, "Leg Right", "Body1", { 0.18f,0,-0.6f });
  hierarchy.add(leg_left, "Leg Left", "Body1", { -0.18f,0,-0.6f });
  hierarchy.add(foot_right, "Foot Right", "Leg Right", { -0.07f,0,0 });
  hierarchy.add(foot_left, "Foot Left", "Leg Left", { -0.07f,0,0 });
}

void create_penguin_cartoon(cgp::hierarchy_mesh_drawable &hierarchy) {
  mesh_drawable body_c;
  mesh_drawable head_c;
  mesh_drawable eye1_c;
  mesh_drawable eye2_c;
  mesh_drawable bec_c;
  mesh_drawable wing_right_c;
  mesh_drawable wing_left_c;
  mesh_drawable foot_right_c;
  mesh_drawable foot_left_c;
  mesh_drawable queue_c;

  body_c.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.5f, 0.5f, 0.7f}));
  head_c.initialize_data_on_gpu(mesh_primitive_sphere(0.35f));
  eye1_c.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.01f, 0.04f, 0.04f}));
  eye1_c.model.rotation = rotation_transform::from_axis_angle({0, 1, 0}, -10 * Pi / 180) * rotation_transform::from_axis_angle({0, 0, 1}, 15 * Pi / 180);
  eye2_c.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.01f, 0.04f, 0.04f}));
  eye2_c.model.rotation = rotation_transform::from_axis_angle({0, 1, 0}, -10 * Pi / 180) * rotation_transform::from_axis_angle({0, 0, 1}, -15 * Pi / 180);
  bec_c.initialize_data_on_gpu(mesh_primitive_tetrahedron({0, -0.15f, -0.1f}, {0, 0, 0.05f}, {0.2f, 0, -0.05f}, {0, 0.15f, -0.1f}));
  wing_right_c.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.2f, 0.01f, 0.4f}));
  wing_right_c.model.rotation = rotation_transform::from_axis_angle({1, 0, 0}, 15 * Pi / 180);
  wing_left_c.initialize_data_on_gpu(mesh_primitive_ellipsoid({0.2f, 0.01f, 0.4f}));
  wing_left_c.model.rotation = rotation_transform::from_axis_angle({1, 0, 0}, -15 * Pi / 180);
  foot_right_c.initialize_data_on_gpu(mesh_primitive_cubic_grid({0, -0.1f, 0}, {0, 0.1f, 0}, {0, 0.1f, 0.15f}, {0, -0.1f, 0.15f}, {0.3f, -0.2f, 0}, {0.3f, 0.2f, 0}, {0.3f, 0.2f, 0.05f}, {0.3f, -0.2f, 0.05f}));
  foot_right_c.model.rotation = rotation_transform::from_axis_angle({0, 0, 1}, 25 * Pi / 180);
  foot_left_c.initialize_data_on_gpu(mesh_primitive_cubic_grid({0, -0.1f, 0}, {0, 0.1f, 0}, {0, 0.1f, 0.15f}, {0, -0.1f, 0.15f}, {0.3f, -0.2f, 0}, {0.3f, 0.2f, 0}, {0.3f, 0.2f, 0.05f}, {0.3f, -0.2f, 0.05f}));
  foot_left_c.model.rotation = rotation_transform::from_axis_angle({0, 0, 1}, -25 * Pi / 180);
  queue_c.initialize_data_on_gpu(mesh_primitive_tetrahedron({0, -0.15f, 0.1f}, {0, 0.15f, 0.1f}, {0, 0, -0.05f}, {-0.2f, 0, 0.05f}));
  queue_c.model.rotation =rotation_transform::from_axis_angle({0, 1, 0}, -30 * Pi / 180);

  auto change2grey = {&eye1_c, &eye2_c, &bec_c, &wing_right_c, &wing_left_c, &foot_right_c, &foot_left_c, &queue_c};
  for (auto m : change2grey)
    m->material.color = {0.3f, 0.3f, 0.3f};
  auto changeNoSpecular = {&body_c, &head_c, &bec_c, &wing_right_c, &wing_left_c, &foot_right_c, &foot_left_c};
  for (auto m : changeNoSpecular)
    m->material.phong.specular = 0;

  body_c.texture.load_and_initialize_texture_2d_on_gpu("../assets/pinguin_cartoon.png");
  head_c.texture.load_and_initialize_texture_2d_on_gpu("../assets/pinguin_cartoon_head2.png");

  hierarchy.add(body_c, "Body Cartoon");
  hierarchy.add(head_c, "Head Cartoon", "Body Cartoon", {0, 0, 0.7f});
  hierarchy.add(eye1_c, "Eye1 Cartoon", "Head Cartoon", {0.33f, 0.1f, 0.05f});
  hierarchy.add(eye2_c, "Eye2 Cartoon", "Head Cartoon", {0.33f, -0.1f, 0.05f});
  hierarchy.add(bec_c, "Bec Cartoon", "Head Cartoon", {0.3f, 0, 0});
  hierarchy.add(wing_right_c, "Wing Right Cartoon", "Body Cartoon",{0, 0.53f, -0.05f});
  hierarchy.add(wing_left_c, "Wing Left Cartoon", "Body Cartoon",{0, -0.53f, -0.05f});
  hierarchy.add(foot_right_c, "Foot Right Cartoon", "Body Cartoon",{0, 0.11f, -0.75f});
  hierarchy.add(foot_left_c, "Foot Left Cartoon", "Body Cartoon",{0, -0.11f, -0.75f});
  hierarchy.add(queue_c, "Queue Cartoon", "Body Cartoon", {-0.3f, 0, -0.55f});
}
