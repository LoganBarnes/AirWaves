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
#include <glm/gtc/constants.hpp>
#include <array>
#include <cmath>

namespace vmp {

/**
 * @brief Sine wave generator.
 */
template <typename T, int frames, int channels>
class SineSource
{
public:
    void create_data(T *buffer, unsigned, unsigned)
    {
        // Write interleaved audio data.
        for (unsigned i = 0; i < frames; i++) {
            for (unsigned j = 0; j < channels; j++) {
                *buffer++ = std::sin(last_values_[j]) * max_amplitude_;
                last_values_[j] += freq_scale_;
                if (last_values_[j] >= glm::pi<T>())
                    last_values_[j] -= glm::two_pi<T>();
            }
        }
    }

    T get_frequency() const { return frequency_; }

    void set_frequency(T freq)
    {
        frequency_ = freq;
        freq_scale_ = frequency_ * glm::two_pi<T>() / vmp::MainStream::instance().get_sample_rate();
    }

private:
    std::array<T, 2> last_values_{{0, 0}};
    static constexpr T max_amplitude_{0.5};
    T frequency_{440.0};
    T freq_scale_{frequency_ * glm::two_pi<T>() / vmp::MainStream::instance().get_sample_rate()};
};

} // namespace
