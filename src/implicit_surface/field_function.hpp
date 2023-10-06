#pragma once

#include "cgp/cgp.hpp"
#include "simulation/simulation.hpp"

struct field_function_structure {
  // Query the value of the function at any point p
  float operator()(cgp::vec3 const& p);

  // ***************************//
  // Parameters
  // ***************************//
  // Particles in the scene
  std::vector<particle_element>* particles;

  // The parameters of the Perlin noise
  float noise_magnitude = 0.06f;  // Magnitude of the noise
  float noise_offset = 0.0f;     // An offset in the parametric domain (get a
                              // different value of noise with same parameters)
  float noise_scale = 1.0f;        // Scale in the parametric domain
  int noise_octave = 5;            // Maximum number of octave
  float noise_persistance = 0.3f;  // Persistence of Perlin noise
  float smooth_factor = 1.f;
  int kernel_number = 0;

  float wendlandKernel(float r);
  float poly6Kernel(float r);
  float spikyKernel(float r);
};
