// ////////////////////////////////////////////////////////////
// Created on 10/21/2017.
// Copyright (c) 2017. All rights reserved.
//  ___________________________$$$$$$$\__________
// | $$\    $$\                $$  __$$\      | ||
// |_$$ |___$$ |_$$\______$$\__$$ |__$$ |_____|_||
// | $$ |   $$ | $$$\    $$$ | $$$$$$$  |  () | ||
// |_\$$\__$$  |_$$$$\__$$$$ |_$$  ____/______|_||
// |  \$$\$$  /  $$\$$\$$ $$ | $$ |        () | ||
// |___\$$$  /___$$ \$$$  $$ |_$$ |___________|_||
// |    \$  /    $$ |\$  /$$ | \__|           | ||
// |_____\_/_____$$ |_\_/_$$ |________________|_||
//               \__|     \__|
// The Visual Music Project
// Created by Logan Barnes
// ////////////////////////////////////////////////////////////
#include "AirWaves.hpp"
#include "Transport.hpp"
#include <vmp/VMP.hpp>
#include <vmp/audio/sources/SawSource.hpp>
#include <vmp/audio/sources/SineSource.hpp>
#include <vmp/VMPConfig.hpp>
#include <gl/GLFunctions.hpp>
#include <imgui.h>

namespace vmp {

AirWaves::AirWaves(int, int) : transport_{std::make_unique<vmp::Transport>()} {
    glpl_.program = gl::create_program(vmp::shader_path() + "orb.vert", vmp::shader_path() + "orb.frag");
}

AirWaves::~AirWaves() = default;

void AirWaves::configure_gui(int, int, bool paused)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (!paused) {
            ImGui::Text("Framerate: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
        }
        transport_->configure_gui();

        ImGui::Separator();
        if (ImGui::VSliderFloat("Master Volume", ImVec2(45, 150), &output_amplitude_, 0.0f, 1.0f)) {
            VMP::Output().set_amplitude(output_amplitude_);
        }

        ImGui::Separator();
        if (ImGui::Button("Add Sine Source")) {
            sines_.emplace_back(VMP::Output().add_source(SineSource<double, 256, 2>{}));
        }
        if (ImGui::Button("Add Sawtooth Source")) {
            saws_.emplace_back(VMP::Output().add_source(SawSource<double, 256, 2>{}));
        }

        ImGui::Separator();
        if (ImGui::Button("Remove Sine Source") && !sines_.empty()) {
            VMP::Output().remove_source(sines_.back());
            sines_.pop_back();
        }
        if (ImGui::Button("Remove Sawtooth Source") && !saws_.empty()) {
            VMP::Output().remove_source(saws_.back());
            saws_.pop_back();
        }
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void AirWaves::render(int, int, double) {}

void AirWaves::play_or_pause()
{
    if (VMP::is_paused()) {
        VMP::resume();
    } else {
        VMP::pause();
    }
}

void AirWaves::stop_or_reset()
{
    if (VMP::is_paused()) {
        VMP::reset();
    } else {
        VMP::pause();
    }
}

} // namespace vmp
