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
#include "RtAudio.h"

int main()
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
            std::cout << "\n\tmaximum output channels = " << info.outputChannels
                      << (info.isDefaultOutput ? " (default)\n" : "\n");
            std::cout << "\tmaximum input channels  = " << info.inputChannels
                      << (info.isDefaultInput ? " (default)\n" : "\n");
            std::cout << "\tmaximum duplex channels = " << info.duplexChannels << "\n";
        }
    }
    return 0;
}