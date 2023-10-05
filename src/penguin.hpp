#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"

class penguin_structure
{
public:
    std::string name;
    mesh mesh_part;
    affine_rts transform_global;
    cgp::vec3 v; // Speed

    float m = 1.0f;     // mass
    penguin_structure(std::string _name, mesh _mesh, affine_rts _t, cgp::vec3 _v = { 0,0,0 }, float _m = 1.0f):
        name(_name), mesh_part(_mesh), transform_global(_t), v(_v), m(_m)
        {}
};

void create_penguin(cgp::hierarchy_mesh_drawable &hierarchy, std::vector<penguin_structure>& penguin);
void penguin_ready(std::vector<penguin_structure>& penguin);
void simulate_penguin(cgp::hierarchy_mesh_drawable &hierarchy, std::vector<penguin_structure>& penguin, float dt);
void update_mesh_drawable(cgp::hierarchy_mesh_drawable &hierarchy, std::vector<penguin_structure>& penguin);

void create_penguin_cartoon(cgp::hierarchy_mesh_drawable &hierarchy);