#include "gui_helper.hpp"

using namespace cgp;

void display_gui_implicit_surface(bool &is_update_field,
                                  bool &is_update_marching_cube,
                                  bool &is_save_obj, gui_parameters &gui,
                                  field_function_structure &field_function) {
  if (ImGui::CollapsingHeader("Display")) {
    ImGui::Checkbox("Surface", &gui.display.surface);
    ImGui::Checkbox("Domain", &gui.display.domain);
  }

  if (ImGui::CollapsingHeader("Domain")) {
    is_update_field |= ImGui::SliderInt("Samples", &gui.domain.samples, 8, 100);

    is_update_field |=
        ImGui::SliderFloat("Lx", &gui.domain.length.x, 0.5f, 10.0f);
    is_update_field |=
        ImGui::SliderFloat("Ly", &gui.domain.length.y, 0.5f, 10.0f);
    is_update_field |=
        ImGui::SliderFloat("Lz", &gui.domain.length.z, 0.5f, 10.0f);
  }

  if (ImGui::CollapsingHeader("Field Function")) {
    ImGui::Text("Noise parameters");
    is_update_field |= ImGui::SliderFloat(
        "Noise Power", &field_function.noise_magnitude, 0.0f, 1.0f);
    is_update_field |= ImGui::SliderFloat(
        "Noise Scale", &field_function.noise_scale, 0.1f, 2.5f);
    is_update_field |=
        ImGui::SliderFloat("Noise offset", &field_function.noise_offset, -3, 3);
    is_update_field |=
        ImGui::SliderInt("Noise Octave", &field_function.noise_octave, 1, 8);
    is_update_field |= ImGui::SliderFloat(
        "Noise Persistance", &field_function.noise_persistance, 0.1f, 0.5f);
  }

    if (ImGui::CollapsingHeader("Marching Cube Function")) {
        is_update_field |= ImGui::SliderFloat(
                "Smoothing Factor", &field_function.smooth_factor, 0.0f, 1.0f);

        is_update_field |= ImGui::RadioButton("Kernel Wendland", &field_function.kernel_number, 0);
        is_update_field |= ImGui::RadioButton("Kernel Poly 6", &field_function.kernel_number, 1);
        is_update_field |= ImGui::RadioButton("Kernel Spiky", &field_function.kernel_number, 2);
    }

  ImGui::Spacing();
  is_update_marching_cube |=
      ImGui::SliderFloat("Isovalue", &gui.isovalue, 0.0f, 1.0f);

  ImGui::Spacing();
  is_save_obj = ImGui::Button("Export mesh as obj");
}
