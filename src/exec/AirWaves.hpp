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

#include <vmp/VmpTypes.hpp>
#include <gl/GLTypes.hpp>
#include <memory>
#include <vector>

namespace vmp {

class AirWaves
{
public:
    AirWaves(int width, int height);
    ~AirWaves();

    void configure_gui(int width, int height, bool paused);
    void render(int width, int height, double alpha);

    void play_or_pause();
    void stop_or_reset();

private:
    std::unique_ptr<vmp::Transport> transport_;
    gl::Pipeline<> glpl_;

    //TODO: should be stored in VMP::Output()
    float output_amplitude_{1.0};

    std::vector<Source> sines_;
    std::vector<Source> saws_;
};

} // namespace vmp
