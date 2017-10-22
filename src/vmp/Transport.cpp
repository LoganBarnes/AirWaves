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
