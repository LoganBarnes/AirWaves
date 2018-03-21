// ////////////////////////////////////////////////////////////
// Created on 3/20/18.
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
#include <memory>
#include <cassert>

namespace gl {

template <typename... Shaders>
Program create_program(const std::string &shader_filename, const Shaders... shader_filenames);

Program create_program(const std::vector<std::string> &shader_filenames);

namespace detail {

template <typename... Shaders>
void append_filenames(std::vector<std::string> *filename_list, const std::string &filename)
{
    assert(filename_list);
    filename_list->emplace_back(filename);
}

template <typename... Shaders, typename = std::enable_if_t<sizeof...(Shaders) != 0>>
void append_filenames(std::vector<std::string> *filename_list, const std::string &filename, const Shaders... filenames)
{
    assert(filename_list);
    filename_list->emplace_back(filename);
    append_filenames(filename_list, filenames...);
}

} // namespace detail

template <typename... Shaders>
Program create_program(const std::string &shader_filename, const Shaders... shader_filenames)
{
    std::vector<std::string> filenames;
    detail::append_filenames(&filenames, shader_filename, shader_filenames...);
    return create_program(filenames);
}

} // namespace gl
