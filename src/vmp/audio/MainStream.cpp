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

    // TODO: store these in VMP::Input() and VMP::Output() respectively?
    std::vector<Sound *> input_sounds;
    std::vector<Sound *> output_sounds;
    double amplitude_{1.0}; // TODO: move this to VMP::Output() and add one for each channel

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
        for (unsigned i = 0; i < nBufferFrames * 2; ++i) {
            outputBuffer[i] += std::min(1.0, std::max(-1.0, outputBuffer[i]));
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
}

MainStream::~MainStream()
{}

} // namespace vmp


