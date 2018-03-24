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
#include "ProgramManager.hpp"
#include <gl/Program.hpp>

namespace gl {

Program ProgramManager::create_program(const std::vector<std::string> &shader_filenames)
{
    return ProgramManager::instance().detail_create_program(shader_filenames);
}

ProgramManager &ProgramManager::instance()
{
    static ProgramManager pm;
    return pm;
}

Program ProgramManager::detail_create_program(const std::vector<std::string> &shader_filenames)
{
    std::string key;
    for (const std::string &filename : shader_filenames) {
        key += filename;
    }

    auto iter = programs_.find(key); // long string hashes may be inefficient so we only do this once
    if (iter != programs_.end()) {
        if (auto program = iter->second.lock()) { // check weak ptr
            return program;
        }
    }

    Program program = std::make_shared<detail::ProgramWrapper>(shader_filenames);
    programs_.emplace(key, program);

    return program;
}

} // namespace gl
