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
#include <unordered_map>
#include <vector>

namespace gl {

namespace detail {

class ProgramManager
{
public:
    static gl::Program create_program(const std::vector<std::string> &shader_filenames);

    static ProgramManager &instance();

    ProgramManager(ProgramManager &) = delete;
    ProgramManager(const ProgramManager &) = delete;
    ProgramManager(ProgramManager &&) noexcept = delete;

    ProgramManager &operator=(ProgramManager &) = delete;
    ProgramManager &operator=(const ProgramManager &) = delete;
    ProgramManager &operator=(ProgramManager &&) noexcept = delete;

private:
    ProgramManager() = default;

    gl::Program detail_create_program(const std::vector<std::string> &shader_filenames);

    std::unordered_map<std::string, std::weak_ptr<detail::ProgramWrapper>> programs_;
};

} // namespace detail

} // namespace gl
