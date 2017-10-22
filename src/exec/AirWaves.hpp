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
#pragma once

#pragma warning(push, 0) // thirdparty headers only
#include <sim-driver/OpenGLSimulation.hpp>
#pragma warning(pop)
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
