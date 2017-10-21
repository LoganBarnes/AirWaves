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
#include "Transport.hpp"
#include "MainStream.hpp"
#include <imgui.h>

namespace vmp
{

void Transport::play()
{
    if (!playing_) {
        MainStream::instance().start_stream();
        playing_ = true;
    }
}

void Transport::pause()
{
    if (playing_) {
        MainStream::instance().stop_stream();
        playing_ = false;
    }
}

void Transport::stop()
{
    if (playing_) {
        MainStream::instance().stop_stream();
        playing_ = false;
    }
}

void Transport::configure_gui()
{
    if (ImGui::CollapsingHeader("Transport", "transport", false, true)) {
        if (ImGui::Button("[]")) { stop(); }
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Stop"); }
        ImGui::SameLine();
        if (playing_) {
            if (ImGui::Button("||")) { pause(); }
            if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Pause"); }
        }
        else {
            if (ImGui::Button(">")) { play(); }
            if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Play"); }
        }
        ImGui::SameLine();
        if (ImGui::Button("<<")) {}
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Back"); }
        ImGui::SameLine();
        if (ImGui::Button(">>")) {}
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Forward"); }
    }
}

} // namespace vmp
