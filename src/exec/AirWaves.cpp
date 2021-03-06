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
#include <glc/Program.hpp>
#include <glc/Buffer.hpp>
#include <glc/VertexArray.hpp>
#include <glc/Framebuffer.hpp>
#include <glc/camera/Camera.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace vmp {

namespace {

const glm::mat4 &identity()
{
    static glm::mat4 ident(1);
    return ident;
}

} // namespace

AirWaves::AirWaves(int width, int height)
    : transport_{std::make_unique<vmp::Transport>()}, camera_{std::make_unique<glc::Camera>()}
{
    glpl_.program = glc::create_program(vmp::shader_path() + "orb.vert", vmp::shader_path() + "orb.frag");

    std::vector<float> quad = {-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.5f, -0.5f, 0.f, 1.f, 0.f,
                               -0.5f, 0.5f,  0.f, 0.f, 1.f, 0.5f, 0.5f,  0.f, 1.f, 1.f};
    glpl_.vbo = glc::create_buffer(quad);

    float *float_ptr = nullptr;
    glpl_.vao = glc::create_vertex_array(glpl_.program,
                                         glpl_.vbo,
                                         sizeof(float) * 5,
                                         {{"world_position", 3, GL_FLOAT, float_ptr},
                                          {"tex_coords", 2, GL_FLOAT, float_ptr + 3}});
    glpl_.fbo = glc::create_framebuffer(static_cast<unsigned>(width), static_cast<unsigned>(height));

    camera_->setAspectRatio(width * 1.f / height);
    camera_->lookAt({0, 0, 5}, {0, 0, 0});
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

        ImGui::Separator();
        ImGui::Text("Sine Waves");
        for (auto &sine : sines_) {
            auto *sine_source = sine.get_as<SineSource<double, 256, 2>>();
            assert(sine_source);

            ImGui::PushID(sine_source);

            auto freq = static_cast<int>(std::nearbyint(sine_source->get_frequency()));
            if (ImGui::DragInt("Frequency", &freq)) {
                sine_source->set_frequency(freq);
            }
            ImGui::PopID();
        }

        ImGui::Separator();
        ImGui::Text("Saw Waves");
        for (auto &saw : saws_) {
            auto *saw_source = saw.get_as<SawSource<double, 256, 2>>();
            assert(saw_source);

            ImGui::PushID(saw_source);

            auto freq = static_cast<int>(std::nearbyint(saw_source->get_frequency()));
            if (ImGui::DragInt("Frequency", &freq)) {
                saw_source->set_frequency(freq);
            }
            ImGui::PopID();
        }
    }
    ImGui::Separator();

    ImGui::Checkbox("TMP use fbo", &tmp_use_fbo_);

    ImGui::End();
    ImGui::PopStyleVar();
}

void AirWaves::render(int, int, double)
{
    glpl_.fbo->use([&] {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glpl_.program->use([&] {
            glpl_.program->set_matrix_uniform("screen_from_world", glm::value_ptr(identity()));
            glpl_.program->set_uniform("use_tex", false);
            glpl_.vao->render(GL_TRIANGLE_STRIP, 0, 4);
        });
    });

    glpl_.program->use([&] {
        glpl_.program->set_matrix_uniform("screen_from_world",
                                          glm::value_ptr(camera_->getPerspectiveScreenFromWorldMatrix()));
        glpl_.program->set_uniform("use_tex", tmp_use_fbo_);
        glpl_.program->set_uniform("tex", glpl_.fbo->get_texture());
        glpl_.vao->render(GL_TRIANGLE_STRIP, 0, 4);
    });
}

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
