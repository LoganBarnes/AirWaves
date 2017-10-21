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
#include "MainStream.hpp"
#include <RtAudio.h>
#include <cmath>
#include <glm/gtc/constants.hpp>

namespace vmp
{

namespace
{

template<typename T>
class Sound
{
public:
    explicit Sound(bool sine = true)
        : sine_(sine)
    {}

    static int play_sound(void *outputBuffer,
                          void *,
                          unsigned int nBufferFrames,
                          double,
                          RtAudioStreamStatus status,
                          void *userData)
    {
        auto *buffer = static_cast<T *>(outputBuffer);
        auto *s = static_cast<Sound<T> *>(userData);

        if (status) {
            std::cout << "Stream underflow detected!" << std::endl;
        }

        return s->play_sound(buffer, nBufferFrames);
    }

private:
    bool sine_;
    std::vector<T> last_values_{0, 0};
    T max_amplitude_{0.2};
    T freq_scale_{0.025};

    int play_sound(T *outputBuffer, unsigned int nBufferFrames)
    {
        return sine_ ? sine(outputBuffer, nBufferFrames) : saw(outputBuffer, nBufferFrames);
    }

    /**
     * @brief Two-channel sawtooth wave generator.
     */
    int saw(T *outputBuffer, unsigned int nBufferFrames)
    {
        // Write interleaved audio data.
        for (unsigned i = 0; i < nBufferFrames; i++) {
            for (unsigned j = 0; j < 2; j++) {
                *outputBuffer++ = last_values_[j];
                last_values_[j] += freq_scale_ * max_amplitude_ / T(2);
                if (last_values_[j] >= max_amplitude_) last_values_[j] -= T(2) * max_amplitude_;
            }
        }
        return 0;
    }

    /**
     * @brief Two-channel sine wave generator.
     */
    int sine(T *outputBuffer, unsigned int nBufferFrames)
    {
        // Write interleaved audio data.
        for (unsigned i = 0; i < nBufferFrames; i++) {
            for (unsigned j = 0; j < 2; j++) {
                *outputBuffer++ = std::sin(last_values_[j]) * max_amplitude_ * T(3);
                last_values_[j] += freq_scale_;
                if (last_values_[j] >= glm::pi<T>()) last_values_[j] -= T(2) * glm::pi<T>();
            }
        }
        return 0;
    }
};

Sound<double> s;
}

MainStream &MainStream::instance()
{
    static MainStream stream;
    return stream;
}

void MainStream::start_stream()
{
    try {
        audio_->startStream();
    }
    catch (const RtAudioError &e) {
        throw std::runtime_error("Failure when starting audio stream. " + e.getMessage());
    }
}

void MainStream::stop_stream()
{
    try {
        audio_->stopStream();
    }
    catch (const RtAudioError &e) {
        throw std::runtime_error("Failure when stopping audio stream. " + e.getMessage());
    }
}

MainStream::MainStream()
    : audio_(std::make_unique<RtAudio>())
{
    RtAudio::StreamParameters parameters;
    parameters.deviceId = audio_->getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned sampleRate = 44100;
    unsigned bufferFrames = 256; // 256 sample frames

    try {
        audio_->openStream(&parameters,
                           nullptr,
                           RTAUDIO_FLOAT64,
                           sampleRate,
                           &bufferFrames,
                           &Sound<double>::play_sound,
                           &s);
    }
    catch (const RtAudioError &e) {
        throw std::runtime_error("Failed to open audio stream. " + e.getMessage());
    }
}

MainStream::~MainStream()
{}

} // namespace vmp
