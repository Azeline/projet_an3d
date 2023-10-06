#include "field_function.hpp"

using namespace cgp;

float field_function_structure::wendlandKernel(float r) {
  float q = r / smooth_factor;
  if (q < 2) {
    return (1.0f - 0.5f * q) * pow(1 - 0.5 * q, 4);
  } else {
    return 0.0f;
  }
}

float field_function_structure::poly6Kernel(float r) {
  float q = r / smooth_factor;
  if (q < 1) {
    return (315.0f / (64.0f * M_PI * pow(smooth_factor, 9))) *
           pow((1 - q * q), 3);
  } else {
    return 0.0;
  }
}

float field_function_structure::spikyKernel(float r){
    float q = r / smooth_factor;
    if (q < 1) {
        return (15.0f / (M_PI * pow(smooth_factor, 6))) * pow((1 - q), 3);
    } else {
        return 0.0;
    }
}

float field_function_structure::operator()(cgp::vec3 const& p) {
  float value = 0.0f;
    float (field_function_structure::*kernelPointer)(float);
    switch (kernel_number) {
        case 0:
            kernelPointer = &field_function_structure::wendlandKernel;
            break;
        case 1:
            kernelPointer = &field_function_structure::poly6Kernel;
            break;
        default:
            kernelPointer = &field_function_structure::spikyKernel;

    }

  for (size_t k = 0; k < particles->size(); ++k) {
      vec3 const& pi = (*particles)[k].p;
      float const r = norm(pi - p);
      value += (this->*kernelPointer)(r);
  }

  if (noise_magnitude > 0 && value > 0) {
    vec3 const offset = vec3{noise_offset + 1000, 1000, 1000};
    vec3 const p_noise = noise_scale * p + offset;
    value += noise_magnitude *
             noise_perlin(p_noise, noise_octave, noise_persistance);
  }

  return value;
}
