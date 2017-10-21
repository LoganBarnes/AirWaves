// ////////////////////////////////////////////////////////////
// Created on 10/21/2017.
// Copyright (c) 2017. All rights reserved.
//  _________________________$$$$$$$\__________
// | $$\    $$\              $$  __$$\      | ||
// |_$$ |   $$ |$$\______$$\_$$ |  $$ |_____|_||
// | $$ |   $$ |$$$\    $$$ |$$$$$$$  |  () | ||
// |_\$$\  $$  |$$$$\__$$$$ |$$  ____/______|_||
// |  \$$\$$  / $$\$$\$$ $$ |$$ |        () | ||
// |___\$$$  /__$$ \$$$  $$ |$$ |___________|_||
// |    \$  /   $$ |\$  /$$ |\__|           | ||
// |_____\_/____$$ |_\_/_$$ |_______________|_||
//              \__|     \__|
// The Virtual Music Project
// Created by Logan Barnes
// ////////////////////////////////////////////////////////////
#include "Transport.hpp"
#include <imgui.h>

namespace vmp
{
void Transport::play()
{

}
void Transport::pause()
{

}
void Transport::stop()
{

}
void Transport::configure_gui()
{
    if (ImGui::CollapsingHeader("Transport", "transport", false, true)) {
        if (ImGui::Button("[]")) { stop(); }
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Stop"); }
        ImGui::SameLine();
        if (ImGui::Button(">")) { play(); }
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Play"); }
        ImGui::SameLine();
        if (ImGui::Button("<<")) {}
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Back"); }
        ImGui::SameLine();
        if (ImGui::Button(">>")) {}
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Forward"); }
    }
}
} // namespace vmp
