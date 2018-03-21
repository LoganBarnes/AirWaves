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

#include <memory>
#include "Sound.hpp"

namespace vmp {

class Source
{
public:
    template <typename T>
    explicit Source(T source);

    Sound *sound() const;

    template <typename T>
    T *detail_data() const;

private:
    template <typename T>
    class SourceSound : public Sound
    {
    public:
        explicit SourceSound(T entity) : data_{std::move(entity)} {}

        void handle_data(double *buffer, unsigned num_frames, unsigned channels) final
        {
            data_.create_data(buffer, num_frames, channels);
        }

        T data_;
    };

    std::shared_ptr<Sound> self_;
};

template <typename T>
Source::Source(T source) : self_(std::make_shared<SourceSound<T>>(std::move(source)))
{}

template <typename T>
T *Source::detail_data() const
{
    return &(static_cast<SourceSound<T> *>(self_.get())->data_);
}

} // namespace vmp
