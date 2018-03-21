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

namespace detail {

class ProgramWrapper
{
public:
    explicit ProgramWrapper(const std::vector<std::string> &shader_filenames);

    template <typename UsageFunc>
    void use(const UsageFunc &usage_func) const;

private:
    std::shared_ptr<GLuint> program_;
};

template <typename UsageFunc>
void ProgramWrapper::use(const UsageFunc &usage_func) const
{
    glUseProgram(*program_);
    usage_func();
}

} // namespace detail

} // namespace gl
