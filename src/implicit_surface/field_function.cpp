#include "field_function.hpp"

using namespace cgp;

float field_function_structure::operator()(cgp::vec3 const& p) const {
  float value = 0.0f;

  for (size_t k = 0; k < particles->size(); ++k) {
    vec3 const& pi = (*particles)[k].p;
    float const r = norm(pi - p);
    value += (1 / r) / particles->size();
  }

  if (noise_magnitude > 0) {
    vec3 const offset = vec3{noise_offset + 1000, 1000, 1000};
    vec3 const p_noise = noise_scale * p + offset;
    value += noise_magnitude *
             noise_perlin(p_noise, noise_octave, noise_persistance);
  }

  return value;
}
