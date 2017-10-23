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
#include <unordered_map>

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
    Source &add_source(T source)
    {
        return add_and_register_source(std::make_unique<Source>(source));
    }

    void remove_source(const Source &source);

    void set_amplitude(double amplitude);

private:
    friend class ::VMP;

    explicit Output(vmp::MainStream &main_stream);
    ~Output() = default;

    vmp::MainStream &main_stream_;
    std::unordered_map<Sound *, std::unique_ptr<Source>> sources_;

    Source &add_and_register_source(std::unique_ptr<Source> source);
};

} // namespace vmp
