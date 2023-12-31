#pragma once

#include "cgp/cgp.hpp"
#include "field_function.hpp"

// Element of the GUI that are not already stored in other structures
struct gui_parameters {
  struct {  // Elements that are displayed
    bool frame = false;
    bool wireframe = false;  // Display the wireframe of the implicit surface
    bool surface = true;     // Display the implicit surface
    bool domain = false;  // Display the box representing the domain of the marching cube
    bool particles = false;
    bool radius = false;
  } display;

  struct {  // Elements of the domain
    int samples = 21;
    cgp::vec3 length = {8, 8, 20.f};
  } domain;

  struct {
      int push_power = 40;
  } penguin_info;

  // Isovalue used during the marching cube
  float isovalue = .7f;
};

void display_gui_implicit_surface(bool &is_update_field, bool &is_update_marching_cube, bool &is_penguin_push, gui_parameters &gui, field_function_structure &field_function);
