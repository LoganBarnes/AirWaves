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
#include "vmp/VMP.hpp"
#include <imgui.h>

namespace vmp
{

void Transport::configure_gui()
{
    if (ImGui::CollapsingHeader("Transport", "transport", false, true)) {
        if (ImGui::Button("[]")) { VMP::reset(); }
        if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Stop"); }
        ImGui::SameLine();
        if (VMP::is_paused()) {
            if (ImGui::Button(">")) { VMP::resume(); }
            if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Play"); }
        }
        else {
            if (ImGui::Button("||")) { VMP::pause(); }
            if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Pause"); }
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
