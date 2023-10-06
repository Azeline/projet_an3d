#pragma once

#include "implicit_surface/implicit_surface.hpp"
#include "penguin.hpp"
#include "simulation/simulation.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc.
// without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
  // ****************************** //
  // Elements and shapes of the scene
  // ****************************** //

  camera_controller_orbit_euler camera_control;
  camera_projection_perspective camera_projection;
  window_structure window;

  mesh_drawable global_frame;         // The standard global frame
  environment_structure environment;  // Standard environment controler
  input_devices
      inputs;  // Storage for inputs status (mouse, keyboard, window dimension)
  gui_parameters gui;  // Standard GUI element storage

  // ****************************** //
  // Elements and shapes of the scene
  // ****************************** //

  // Timer used for the animation
  timer_basic timer;

    sph_parameters_structure sph_parameters; // Physical parameter related to SPH
    std::vector<particle_element> particles;      // Storage of the particles
    cgp::mesh_drawable sphere_particle; // Sphere used to display a particle
    cgp::curve_drawable curve_visual;   // Circle used to display the radius h of influence

  // The entire hierarchy
  cgp::hierarchy_mesh_drawable hierarchy;
  penguin_structure penguin_struct;

  // Water components
  implicit_surface_structure implicit_surface; // Structures used for the implicit surface (*)
  field_function_structure field_function;     // A Parametric function used to generate the discrete field (*)

  // ****************************** //
  // Functions
  // ****************************** //

  void initialize();  // Standard initialization to be called before the
                      // animation loop
  void
  display_frame();  // The frame display to be called within the animation loop
  void display_gui();  // The display of the GUI, also called within the
                       // animation loop
  void initialize_sph();

  void mouse_move_event();
  void mouse_click_event();
  void keyboard_event();
  void idle_frame();
};
