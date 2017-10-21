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
#pragma once

#include <sim-driver/OpenGLSimulation.hpp>
#include <vmp/Transport.hpp>

namespace vmp
{

class AirWaves
{
public:
    AirWaves(int w, int h, sim::SimData *pSimData);

    void onGuiRender(int w, int h);

private:
    sim::SimData &simData_;

    vmp::Transport transport_;
};

} // namespace vmp
