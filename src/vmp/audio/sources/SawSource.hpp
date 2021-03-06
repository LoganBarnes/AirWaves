// ////////////////////////////////////////////////////////////
// Created on 10/23/17.
// Copyright (c) 2017. All rights reserved.
//
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
// Created by Logan T. Barnes
// ////////////////////////////////////////////////////////////
#pragma once

#include <vmp/audio/MainStream.hpp>
#include <array>

namespace vmp {

/**
 * @brief Sawtooth wave generator.
 */
template <typename T, int frames, int channels>
class SawSource
{
public:
    void create_data(T *buffer, unsigned, unsigned)
    {
        // Write interleaved audio data.
        for (unsigned i = 0; i < frames; i++) {
            for (unsigned j = 0; j < channels; j++) {
                *buffer++ = last_values_[j] * max_amplitude_;
                last_values_[j] += freq_scale_;
                if (last_values_[j] >= 1.0)
                    last_values_[j] -= 2.0;
            }
        }
    }

    T get_frequency() const { return frequency_; }

    void set_frequency(T freq)
    {
        frequency_ = freq;
        freq_scale_ = frequency_ * 2.f / vmp::MainStream::instance().get_sample_rate();
    }

private:
    std::array<T, 2> last_values_{{0, 0}};
    static constexpr T max_amplitude_{0.2};
    T frequency_{440.0};
    T freq_scale_{frequency_ * 2.f / vmp::MainStream::instance().get_sample_rate()};
};

} // namespace
