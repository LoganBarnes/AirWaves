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

#include <sim-driver/SimData.hpp>
#include <vmp/VmpTypes.hpp>
#include <memory>

namespace vmp
{

class AirWaves
{
public:
    AirWaves(int width, int height, sim::SimData *pSimData);
    ~AirWaves();

    void onGuiRender(int width, int height);

private:
    sim::SimData &simData_;
    std::unique_ptr<vmp::Transport> transport_;

    //TODO: should be stored in VMP::Output()
    float output_amplitude_{1.0};
};

} // namespace vmp
