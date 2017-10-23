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
#include <vmp/audio/Source.hpp>
#include <memory>
#include <vector>

namespace vmp
{

class Output
{
public:
    // There can only be one!
    Output(const Output &) = delete;
    Output(Output &&) noexcept = delete;
    Output &operator=(const Output &) = delete;
    Output &operator=(Output &&) noexcept = delete;

    template<typename T>
    Source add_source(T source)
    {
        sources_.emplace_back(std::make_unique<Source>(source));
        register_source(sources_.back());
        return *sources_.back();
    }

    void set_amplitude(double amplitude);

private:
    friend class ::VMP;

    explicit Output(vmp::MainStream &main_stream);
    ~Output() = default;

    vmp::MainStream &main_stream_;
    std::vector<std::unique_ptr<Source>> sources_;

    void register_source(const std::unique_ptr<Source> &source);
};

} // namespace vmp
