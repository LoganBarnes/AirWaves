// ////////////////////////////////////////////////////////////
// Created on 2/25/18.
// Copyright (c) 2018. All rights reserved.
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

#include <gl/GLTypes.hpp>
#include <vector>

namespace gl {

using IdVec = std::vector<std::shared_ptr<GLuint>>;

template <typename... Shaders>
IdVec create_shaders(const std::string &shader_filename, const Shaders... shader_filenames);

std::shared_ptr<GLuint> create_shader(const std::string &shader_filename);

class Program
{
public:
    template <typename... Shaders>
    explicit Program(const std::string &shader_filename, const Shaders... shader_filenames);
    explicit Program(const std::vector<std::string> &shader_filenames);

private:
    std::shared_ptr<GLuint> program_;
};

template <typename... Shaders>
Program::Program(const std::string &shader_filename, const Shaders... shader_filenames)
{
    IdVec shaders = create_shaders(shader_filename, shader_filenames...);
}

template <typename... Shaders>
IdVec create_shaders(const std::string &shader_filename, const Shaders... shader_filenames)
{
    IdVec shaders = (sizeof...(Shaders) != 0 ? create_shaders(shader_filenames...) : IdVec{});

    // create/compile shader and add it to list of shaders
    shaders.insert(shaders.begin(), create_shader(shader_filename));
}

} // namespace gl
