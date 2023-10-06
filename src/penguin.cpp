#include "penguin.hpp"

using namespace cgp;

void update_mesh_drawable(cgp::hierarchy_mesh_drawable &hierarchy,
                          penguin_structure &penguin) {
  for (int i = 0; i < penguin.mesh_parts.size(); i++) {
    penguin.mesh_parts[i].normal_update();
    hierarchy[penguin.names[i]].drawable.vbo_position.update(
        penguin.mesh_parts[i].position.data);
    hierarchy[penguin.names[i]].drawable.vbo_normal.update(penguin.mesh_parts[i].normal.data);
  }
}

void get_bounding_box(cgp::hierarchy_mesh_drawable &hierarchy, penguin_structure& penguin) {
  for (int i = 0; i < penguin.mesh_parts.size(); i++) {
    auto to_global = penguin.transform_globals[i].matrix() * hierarchy[penguin.names[i]].drawable.model.matrix();
    for (int j = 0; j < penguin.mesh_parts[i].position.size(); j++) {
      auto curr_pt = to_global * numarray_stack<float, 4>(penguin.mesh_parts[i].position[j], 1.0);
      penguin.bounding_min.x = std::min(penguin.bounding_min.x, curr_pt.x);
      penguin.bounding_min.y = std::min(penguin.bounding_min.y, curr_pt.y);
      penguin.bounding_min.z = std::min(penguin.bounding_min.z, curr_pt.z);

      penguin.bounding_max.x = std::max(penguin.bounding_max.x, curr_pt.x);
      penguin.bounding_max.y = std::max(penguin.bounding_max.y, curr_pt.y);
      penguin.bounding_max.z = std::max(penguin.bounding_max.z, curr_pt.z);
    }
  }
}

void create_penguin(cgp::hierarchy_mesh_drawable &hierarchy, penguin_structure& penguin) {
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

  mesh m_body1 = mesh_primitive_ellipsoid({ 0.4f,0.45f,0.75f });
  body1.initialize_data_on_gpu(m_body1);
  m_body1.apply_rotation_to_position({0,1,0},40*Pi/180);
  m_body1.apply_rotation_to_position({0,0,1},-180*Pi/180);
  mesh m_body2 = mesh_primitive_ellipsoid({ 0.32f,0.28f,0.25f });
  body2.initialize_data_on_gpu(m_body2);
  m_body2.apply_rotation_to_position({1,0,0},-10*Pi/180);
  mesh m_head1 = mesh_primitive_ellipsoid({ 0.25f,0.25f,0.33f });
  head1.initialize_data_on_gpu(m_head1);
  m_head1.apply_rotation_to_position({0,1,0},10*Pi/180);
  m_head1.apply_rotation_to_position({0,0,1},-180*Pi/180);
  mesh m_head2 = mesh_primitive_ellipsoid({ 0.17f,0.19f,0.26f });
  head2.initialize_data_on_gpu(m_head2);
  m_head2.apply_rotation_to_position({0,1,0},90*Pi/180);
  m_head2.apply_rotation_to_position({0,0,1},-180*Pi/180);
  mesh m_eye1 = mesh_primitive_ellipsoid({ 0.035f,0.01f,0.03f });
  eye1.initialize_data_on_gpu(m_eye1);
  m_eye1.apply_rotation_to_position({1,0,0},20*Pi/180);
  m_eye1.apply_rotation_to_position({0,0,1},15*Pi/180);
  mesh m_eye2 = mesh_primitive_ellipsoid({ 0.035f,0.01f,0.03f });
  eye2.initialize_data_on_gpu(m_eye2);
  m_eye2.apply_rotation_to_position({1,0,0},-20*Pi/180);
  m_eye2.apply_rotation_to_position({0,0,1},-15*Pi/180);
  mesh m_bec = mesh_primitive_cubic_grid({ -0.35f,0.05f,-0.03f }, { -0.3f,0,0.02f }, { -0.35f,-0.05f,-0.03f }, { -0.4f,0,-0.08f }, { 0,0.1f,0 }, { 0,0,0.1f }, { 0,-0.1f,0 }, { 0,0,-0.1f });
  bec.initialize_data_on_gpu(m_bec);
  mesh m_wing_right = mesh_primitive_ellipsoid({ 0.1f,0.01f,0.4f });
  wing_right.initialize_data_on_gpu(m_wing_right);
  m_wing_right.apply_rotation_to_position({0,1,0},-30*Pi/180);
  m_wing_right.apply_rotation_to_position({1,0,0},-15*Pi/180);
  mesh m_wing_left = mesh_primitive_ellipsoid({ 0.1f,0.01f,0.4f });
  wing_left.initialize_data_on_gpu(m_wing_left);
  m_wing_left.apply_rotation_to_position({0,1,0},-30*Pi/180);
  m_wing_left.apply_rotation_to_position({1,0,0},15*Pi/180);
  mesh m_queue = mesh_primitive_ellipsoid({ 0.2f,0.25f,0.4f });
  queue.initialize_data_on_gpu(m_queue);
  m_queue.apply_rotation_to_position({0,1,0},-70*Pi/180);
  mesh m_leg_right = mesh_primitive_cylinder(0.07f, { 0,0,0 }, { 0,0,0.2f }, 10, 20, true);
  leg_right.initialize_data_on_gpu(m_leg_right);
  mesh m_leg_left = mesh_primitive_cylinder(0.07f, { 0,0,0 }, { 0,0,0.2f }, 10, 20, true);
  leg_left.initialize_data_on_gpu(m_leg_left);
  mesh m_foot_right = mesh_primitive_cubic_grid({ 0,0,0.06f }, { 0,-0.14f,0.06f }, { -0.3f,-0.24f,0.03f }, { -0.3f,0.08f,0.03f }, { 0,0,0 }, { 0,-0.14f,0 }, { -0.3f,-0.24f,0 }, { -0.3f,0.08f,0 });
  foot_right.initialize_data_on_gpu(m_foot_right);
  mesh m_foot_left = mesh_primitive_cubic_grid({ 0,0,0.06f }, { 0,-0.14f,0.06f }, { -0.3f,-0.22f,0.03f }, { -0.3f,0.1f,0.03f }, { 0,0,0 }, { 0,-0.14f,0 }, { -0.3f,-0.22f,0 }, { -0.3f,0.1f,0 });
  foot_left.initialize_data_on_gpu(m_foot_left);

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

  hierarchy.add(body1, "Body1", "Floor", { 0,0,1.7f });
  hierarchy.add(body2, "Body2", "Body1", { -0.25f,0,0.5f });
  hierarchy.add(head1, "Head1", "Body2", { 0,0,0.12f });
  hierarchy.add(head2, "Head2", "Head1", { -0.08f,0,0.16f });
  hierarchy.add(eye1, "Eye1", "Head2", {-0.1f,0.165f,0.05f});
  hierarchy.add(eye2, "Eye2", "Head2", {-0.1f,-0.165f,0.05f});
  hierarchy.add(bec, "Bec", "Head2", { -0.1f,0,0 });
  hierarchy.add(wing_right, "Wing Right", "Body1", { 0,-0.46f,0 });
  hierarchy.add(wing_left, "Wing Left", "Body1", { 0,0.46f,0 });
  hierarchy.add(queue, "Queue", "Body1", { 0.25f,0,-0.4f });
  hierarchy.add(leg_right, "Leg Right", "Body1", { 0,-0.18f,-0.6f });
  hierarchy.add(leg_left, "Leg Left", "Body1", { 0,0.18f,-0.6f });
  hierarchy.add(foot_right, "Foot Right", "Leg Right", { 0,0.07f,0 });
  hierarchy.add(foot_left, "Foot Left", "Leg Left", { 0,0.07f,0 });

  std::vector<std::string> tmp_names = { "Body1", "Body2", "Head1", "Head2", "Eye1", "Eye2", "Bec", "Wing Right", "Wing Left", "Queue", "Leg Right", "Leg Left", "Foot Right", "Foot Left" };
  std::vector<mesh> tmp_mesh = { m_body1, m_body2, m_head1, m_head2, m_eye1, m_eye2, m_bec, m_wing_right, m_wing_left, m_queue, m_leg_right, m_leg_left, m_foot_right, m_foot_left };
  std::vector<affine_rts> tmp_transform;
  tmp_transform.push_back(affine_rts(rotation_transform(), { 0,0,1.7f }, 1.0f));
  tmp_transform.push_back(tmp_transform[0] * affine_rts(rotation_transform(), { -0.25f,0,0.5f }, 1.0f));
  tmp_transform.push_back(tmp_transform[1] * affine_rts(rotation_transform(), { 0,0,0.12f }, 1.0f));
  tmp_transform.push_back(tmp_transform[2] * affine_rts(rotation_transform(), { -0.08f,0,0.16f }, 1.0f));
  tmp_transform.push_back(tmp_transform[3] * affine_rts(rotation_transform(), {-0.1f,0.165f,0.05f}, 1.0f));
  tmp_transform.push_back(tmp_transform[3] * affine_rts(rotation_transform(), {-0.1f,-0.165f,0.05f}, 1.0f));
  tmp_transform.push_back(tmp_transform[3] * affine_rts(rotation_transform(), { -0.1f,0,0 }, 1.0f));
  tmp_transform.push_back(tmp_transform[0] * affine_rts(rotation_transform(), { 0,-0.46f,0 }, 1.0f));
  tmp_transform.push_back(tmp_transform[0] * affine_rts(rotation_transform(), { 0,0.46f,0 }, 1.0f));
  tmp_transform.push_back(tmp_transform[0] * affine_rts(rotation_transform(), { 0.25f,0,-0.4f }, 1.0f));
  tmp_transform.push_back(tmp_transform[0] * affine_rts(rotation_transform(), { 0,-0.18f,-0.6f }, 1.0f));
  tmp_transform.push_back(tmp_transform[0] * affine_rts(rotation_transform(), { 0,0.18f,-0.6f }, 1.0f));
  tmp_transform.push_back(tmp_transform[10] * affine_rts(rotation_transform(), { 0,0.07f,0 }, 1.0f));
  tmp_transform.push_back(tmp_transform[11] * affine_rts(rotation_transform(), { 0,0.07f,0 }, 1.0f));
  penguin = penguin_structure(tmp_names, tmp_mesh, tmp_transform, { 0,0,1.7f });

  update_mesh_drawable(hierarchy, penguin);
}

void penguin_ready(cgp::hierarchy_mesh_drawable &hierarchy, penguin_structure& penguin) {
  penguin.mesh_parts[0].apply_rotation_to_position({0,1,0},-50*Pi/180);

  penguin.mesh_parts[1].apply_rotation_to_position({0,1,0},-50*Pi/180);
  penguin.mesh_parts[1].apply_translation_to_position({ -0.25f,0,-0.4f });

  penguin.mesh_parts[2].apply_rotation_to_position({0,1,0},-10*Pi/180);
  penguin.mesh_parts[2].apply_translation_to_position({ -0.35f,0,-0.4f });

  penguin.mesh_parts[3].apply_translation_to_position({ -0.45f,0,-0.4f });
  penguin.mesh_parts[4].apply_translation_to_position({ -0.45f,0,-0.4f });
  penguin.mesh_parts[5].apply_translation_to_position({ -0.45f,0,-0.4f });
  penguin.mesh_parts[6].apply_translation_to_position({ -0.45f,0,-0.4f });

  penguin.mesh_parts[7].apply_rotation_to_position({1,0,0},-45*Pi/180);
  penguin.mesh_parts[7].apply_rotation_to_position({0,1,0},-20*Pi/180);
  penguin.mesh_parts[7].apply_rotation_to_position({0,0,1},20*Pi/180);
  penguin.mesh_parts[7].apply_translation_to_position({ 0.05f,-0.06f,0 });
  penguin.mesh_parts[8].apply_rotation_to_position({1,0,0},45*Pi/180);
  penguin.mesh_parts[8].apply_rotation_to_position({0,1,0},-20*Pi/180);
  penguin.mesh_parts[8].apply_rotation_to_position({0,0,1},-20*Pi/180);
  penguin.mesh_parts[8].apply_translation_to_position({ 0.05f,0.06f,0 });

  penguin.mesh_parts[9].apply_rotation_to_position({0,1,0},-10*Pi/180);
  penguin.mesh_parts[9].apply_translation_to_position({ 0.17f,0,0.42f });

  penguin.mesh_parts[10].apply_rotation_to_position({0,1,0},-80*Pi/180);
  penguin.mesh_parts[10].apply_translation_to_position({ 0.65f,0,0.35f });
  penguin.mesh_parts[11].apply_rotation_to_position({0,1,0},-80*Pi/180);
  penguin.mesh_parts[11].apply_translation_to_position({ 0.65f,0,0.35f });

  penguin.mesh_parts[12].apply_rotation_to_position({0,1,0},-150*Pi/180);
  penguin.mesh_parts[12].apply_translation_to_position({ 0.65f,0,0.38f });
  penguin.mesh_parts[13].apply_rotation_to_position({0,1,0},-150*Pi/180);
  penguin.mesh_parts[13].apply_translation_to_position({ 0.65f,0,0.38f });

  get_bounding_box(hierarchy, penguin);
  update_mesh_drawable(hierarchy, penguin);
}

void simulate_penguin(cgp::hierarchy_mesh_drawable &hierarchy, penguin_structure& penguin, float dt) {
	vec3 const g = { 0,0,-9.81f };
  penguin.v = (1 - 0.9f * dt) * penguin.v + dt * (penguin.m * g);
  auto new_p = penguin.bounding_min + dt * penguin.v;
  if (new_p.z < 1 && ((new_p.x > -25+4) || (new_p.y > 4)||(new_p.y < -4))) {
    new_p.z = 1;
  }
  vec3 to_move = new_p - penguin.bounding_min;
  penguin.bounding_min += to_move;
  penguin.bounding_max += to_move;
  penguin.center += to_move;
  hierarchy["Body1"].transform_local.translation = penguin.center;
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

  hierarchy.add(body_c, "Body Cartoon", "Floor", { 0,0,1 });
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
