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
#include "AirWaves.hpp"

int main()
{
    try {
        sim::OpenGLSimulation<vmp::AirWaves>{{"Air Waves"}}.runNoFasterThanRealTimeLoop();
    }
    catch (const std::exception &e) {
        std::cerr << "ERROR program failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
