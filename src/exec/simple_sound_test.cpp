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
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <thread>

namespace {
void print_usage()
{
    std::cout << "Usage:\n"
              << "./sound-test [options]\n\n"
              << "Options:\n"
              << "\t-h,--help   show this message\n"
              << "\t-s,--sine   play sine waveform (default)\n"
              << "\t-w,--saw    play sawtooth waveform" << std::endl;
}
} // namespace

template <typename T>
class Sound
{
public:
    explicit Sound(bool sine = true) : sine_(sine) {}

    static int play_sound(
        void *outputBuffer, void *, unsigned int nBufferFrames, double, RtAudioStreamStatus status, void *userData)
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
                if (last_values_[j] >= max_amplitude_)
                    last_values_[j] -= T(2) * max_amplitude_;
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
                if (last_values_[j] >= glm::pi<T>())
                    last_values_[j] -= T(2) * glm::pi<T>();
            }
        }
        return 0;
    }
};

int main(const int argc, const char *argv[])
{
    bool use_sine = true;
    for (int argi = 1; argi < argc; ++argi) {
        std::string arg(argv[argi]);
        if (arg == "-s" || arg == "--sine") {
            use_sine = true;
        } else if (arg == "-w" || arg == "--saw") {
            use_sine = false;
        } else if (arg == "-h" || arg == "--help") {
            print_usage();
            return EXIT_SUCCESS;
        } else {
            std::cout << "Unrecognised option: '" << arg << "'\n";
            print_usage();
            return EXIT_SUCCESS;
        }
    }
    RtAudio dac;
    if (dac.getDeviceCount() < 1) {
        std::cout << "\nNo audio devices found!\n";
        return EXIT_FAILURE;
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;
    unsigned sampleRate = 44100;
    unsigned bufferFrames = 256; // 256 sample frames
    Sound<double> s(use_sine);
    try {
        dac.openStream(&parameters,
                       nullptr,
                       RTAUDIO_FLOAT64,
                       sampleRate,
                       &bufferFrames,
                       &Sound<double>::play_sound,
                       &s);
        dac.startStream();
    } catch (const RtAudioError &e) {
        e.printMessage();
        return EXIT_FAILURE;
    }

    char input;
    std::cout << "\nPlaying ... press <enter> to quit.\n";
    std::cin.get(input);
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    try {
        // Stop the stream
        dac.stopStream();
    } catch (const RtAudioError &e) {
        e.printMessage();
        return EXIT_FAILURE;
    }
    if (dac.isStreamOpen()) {
        dac.closeStream();
    }

    return EXIT_SUCCESS;
}