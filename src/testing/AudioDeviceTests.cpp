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
#include <RtAudio.h>
#include <gtest/gtest.h>

namespace
{

TEST(RelationUnitTests, LevelsAreCorrect)
{
    RtAudio audio;
    // Determine the number of devices available
    unsigned int devices = audio.getDeviceCount();
    // Scan through devices for various capabilities
    RtAudio::DeviceInfo info;
    for (unsigned int i = 0; i < devices; i++) {
        info = audio.getDeviceInfo(i);
        if (info.probed) {
            // Print, for example, the maximum number of output channels for each device
            std::cout << "device = " << i;
            std::cout << ": maximum output channels = " << info.outputChannels << "\n";
        }
    }
}

} // namespace