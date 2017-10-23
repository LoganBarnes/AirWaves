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
#include <iostream>
#include "Output.hpp"
#include "MainStream.hpp"

namespace vmp
{

Output::Output(vmp::MainStream &main_stream)
    : main_stream_(main_stream)
{}

void Output::set_amplitude(double amplitude)
{
    vmp::MainStream::instance().set_output_amplitude(amplitude);
}

void Output::remove_source(const Source &source)
{
    auto iter = sources_.find(source.sound());
    if (iter == sources_.end()) {
        std::cerr << "WARNING: Attempting to remove a source that doesn't exist" << std::endl;
        return;
    }
    vmp::MainStream::instance().remove_output_sound(source.sound());
    sources_.erase(iter);
}

Source &Output::add_and_register_source(std::unique_ptr<Source> source)
{
    vmp::MainStream::instance().add_output_sound(source->sound());
    auto iter = sources_.emplace(source->sound(), std::move(source)).first;
    return *(iter->second);
}

} // namespace vmp
