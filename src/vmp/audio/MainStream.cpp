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
#include "Source.hpp"
#include "Sound.hpp"
#include <RtAudio.h>
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <cstring>

namespace vmp
{

namespace
{

/**
 * @brief Two-channel sawtooth wave generator.
 */
class SawSource
{
public:
    SawSource() = default;
    void create_data(double *buffer, unsigned num_frames, unsigned channels)
    {
        // Write interleaved audio data.
        for (unsigned i = 0; i < num_frames; i++) {
            for (unsigned j = 0; j < channels; j++) {
                *buffer++ = last_values_[j];
                last_values_[j] += freq_scale_ * max_amplitude_ / 2.0;
                if (last_values_[j] >= max_amplitude_) last_values_[j] -= 2.0 * max_amplitude_;
            }
        }
    }
private:
    std::array<double, 2> last_values_{{0, 0}};
    double max_amplitude_{0.2};
    double freq_scale_{0.025};
};

/**
 * @brief Two-channel sine wave generator.
 */
class SineSource
{
public:
    SineSource() = default;
    void create_data(double *buffer, unsigned num_frames, unsigned channels)
    {
        // Write interleaved audio data.
        for (unsigned i = 0; i < num_frames; i++) {
            for (unsigned j = 0; j < channels; j++) {
                *buffer++ = std::sin(last_values_[j]) * max_amplitude_ * double(3);
                last_values_[j] += freq_scale_;
                if (last_values_[j] >= glm::pi<double>()) last_values_[j] -= double(2) * glm::pi<double>();
            }
        }
    }
private:
    std::array<double, 2> last_values_{{0, 0}};
    double max_amplitude_{0.2};
    double freq_scale_{0.025};
};

Source &saw_source()
{
    SawSource saw;
    static Source s(saw);
    return s;
}

Source &sine_source()
{
    SineSource sine;
    static Source s(sine);
    return s;
}

} // namespace

class MainStream::AudioStream
{
public:
    explicit AudioStream(unsigned frame_size)
        : buffer_(frame_size * 2)
    {}

    static int audio_callback(void *outputBuffer,
                              void *,
                              unsigned int nBufferFrames,
                              double,
                              RtAudioStreamStatus status,
                              void *userData)
    {
        auto *buffer = static_cast<double *>(outputBuffer);
        auto *s = static_cast<AudioStream *>(userData);

        if (status) {
            std::cout << "Stream underflow detected!" << std::endl;
        }

        return s->play_sound(buffer, nBufferFrames);
    }

    std::vector<Sound *> input_sounds;
    std::vector<Sound *> output_sounds;
    double amplitude_{1.0};

private:
    std::vector<double> buffer_;

    int play_sound(double *outputBuffer, unsigned int nBufferFrames)
    {
        std::memset(outputBuffer, 0, sizeof(double) * nBufferFrames * 2);

        for (Sound *s : output_sounds) {
            s->handle_data(buffer_.data(), nBufferFrames, 2);
            for (unsigned i = 0; i < nBufferFrames * 2; ++i) {
                outputBuffer[i] += buffer_[i] * amplitude_;
            }
        }
        return 0;
    }
};

MainStream &MainStream::instance()
{
    static MainStream stream;
    return stream;
}

void MainStream::start_stream()
{
    if (stream_running_) {
        return;
    }
    try {
        audio_->startStream();
    }
    catch (const RtAudioError &e) {
        throw std::runtime_error("Failure when starting audio stream. " + e.getMessage());
    }
    stream_running_ = true;
}

void MainStream::stop_stream()
{
    if (!stream_running_) {
        return;
    }
    try {
        audio_->stopStream();
    }
    catch (const RtAudioError &e) {
        throw std::runtime_error("Failure when stopping audio stream. " + e.getMessage());
    }
    stream_running_ = false;
}

bool MainStream::is_stream_running() const
{
    return stream_running_;
}

void MainStream::add_input_sound(Sound *sound)
{
    stream_->input_sounds.emplace_back(sound);
}

void MainStream::add_output_sound(Sound *sound)
{
    stream_->output_sounds.emplace_back(sound);
}

void MainStream::set_output_amplitude(double amplitude)
{
    stream_->amplitude_ = std::min(1.2, std::max(0.0, amplitude));
}

MainStream::MainStream()
    : audio_(std::make_unique<RtAudio>()),
      stream_{nullptr}
{
    RtAudio::StreamParameters parameters;

    parameters.deviceId = audio_->getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

    unsigned sampleRate = 44100;
    unsigned bufferFrames = 256; // 256 sample frames

    stream_ = std::make_unique<AudioStream>(bufferFrames);

    try {
        audio_->openStream(&parameters,
                           nullptr,
                           RTAUDIO_FLOAT64,
                           sampleRate,
                           &bufferFrames,
                           &MainStream::AudioStream::audio_callback,
                           stream_.get());
    }
    catch (const RtAudioError &e) {
        throw std::runtime_error("Failed to open audio stream. " + e.getMessage());
    }
    add_output_sound(saw_source().sound());
    add_output_sound(sine_source().sound());
}

MainStream::~MainStream()
{}

} // namespace vmp


