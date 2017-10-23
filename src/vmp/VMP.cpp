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
#include "VMP.hpp"
#include <vmp/audio/MainStream.hpp>

void VMP::resume()
{
    vmp::MainStream::instance().start_stream();
}
void VMP::pause()
{
    vmp::MainStream::instance().stop_stream();
}
void VMP::reset()
{
    vmp::MainStream::instance().stop_stream();
}
bool VMP::is_paused()
{
    return !vmp::MainStream::instance().is_stream_running();
}
vmp::Output &VMP::Output()
{
    static vmp::Output output(vmp::MainStream::instance());
    return output;
}
