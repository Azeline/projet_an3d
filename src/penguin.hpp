#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"

class penguin_structure {
 public:
  std::vector<std::string> names;
  std::vector<mesh> mesh_parts;
  std::vector<affine_rts> transform_globals;
  cgp::vec3 center;
  cgp::vec3 bounding_min;
  cgp::vec3 bounding_max;
  cgp::vec3 v;  // Speed

  float m = 1.0f;  // mass
  penguin_structure() = default;
  penguin_structure(std::vector<std::string> _names, std::vector<mesh> _mesh, std::vector<affine_rts> _t, cgp::vec3 _c, cgp::vec3 _b_min = { INFINITY,INFINITY,INFINITY }, cgp::vec3 _b_max = { -INFINITY,-INFINITY,-INFINITY }, cgp::vec3 _v = {0, 0, 0}, float _m = 1.0f)
      : names(_names), mesh_parts(_mesh), transform_globals(_t), center(_c), bounding_min(_b_min), bounding_max(_b_max), v(_v), m(_m) {}
};

void create_penguin(cgp::hierarchy_mesh_drawable &hierarchy, penguin_structure& penguin);
void penguin_ready(cgp::hierarchy_mesh_drawable& hierarchy, penguin_structure& penguin);
void simulate_penguin(cgp::hierarchy_mesh_drawable &hierarchy, penguin_structure& penguin, float dt);
void get_bounding_box(cgp::hierarchy_mesh_drawable &hierarchy, penguin_structure& penguin);

void create_penguin_cartoon(cgp::hierarchy_mesh_drawable &hierarchy);
