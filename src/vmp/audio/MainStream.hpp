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
#include <memory>
#include <vector>

class RtAudio;

namespace vmp
{

class MainStream
{
public:
    static MainStream &instance();

    void start_stream();
    void stop_stream();
    bool is_stream_running() const;

    void add_input_sound(Sound *sound);
    void add_output_sound(Sound *sound);

    void set_output_amplitude(double amplitude);

private:
    MainStream();
    ~MainStream();

    class AudioStream;
    std::unique_ptr<RtAudio> audio_;
    std::unique_ptr<AudioStream> stream_;

    bool stream_running_{false};
};

} // namespace vmp
