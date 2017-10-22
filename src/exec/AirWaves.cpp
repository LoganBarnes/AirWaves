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

namespace vmp
{

AirWaves::AirWaves(int, int, sim::SimData *pSimData)
    : simData_(*pSimData)
{
}
void AirWaves::onGuiRender(int, int)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Framerate: %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
        transport_.configure_gui();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

} // namespace vmp
