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
#include <gl/Program.hpp>
#include <vmp/VMPConfig.hpp>
#include "GLInstance.hpp"
#include <gmock/gmock.h>

namespace {

class ProgramTests : public ::testing::Test
{
    gl::testing::GLInstance gl_instance_;
};

TEST_F(ProgramTests, SameShadersReturnSameProgram)
{
    gl::Program program1 = gl::create_program(
        {vmp::testing::shader_path() + "shader1.vert", vmp::testing::shader_path() + "shader1.frag"});

    gl::Program program2 = gl::create_program(
        {vmp::testing::shader_path() + "shader1.vert", vmp::testing::shader_path() + "shader2.frag"});

    gl::Program program3 = gl::create_program(
        {vmp::testing::shader_path() + "shader1.vert", vmp::testing::shader_path() + "shader1.frag"});

    gl::Program program4 = gl::create_program(
        {vmp::testing::shader_path() + "shader1.vert", vmp::testing::shader_path() + "shader2.frag"});

    EXPECT_EQ(program1, program3);
    EXPECT_EQ(program2, program4);

    EXPECT_NE(program1, program2);
}

TEST_F(ProgramTests, ProgramIsDeletedOutOfScope)
{
    gl::detail::ProgramWrapper *orig_ptr;
    {
        gl::Program program1 = gl::create_program(
            {vmp::testing::shader_path() + "shader1.vert", vmp::testing::shader_path() + "shader1.frag"});

        orig_ptr = program1.get();
    }
    // no more shared pointers for program1. Next program with same shaders should be different.

    // create another program with different shaders just to make sure the previous memory isn't used again
    gl::Program other_program = gl::create_program(
        {vmp::testing::shader_path() + "shader1.vert", vmp::testing::shader_path() + "shader2.frag"});

    // same shaders as program1 but the object should be different now
    gl::Program program2 = gl::create_program(
        {vmp::testing::shader_path() + "shader1.vert", vmp::testing::shader_path() + "shader1.frag"});

    EXPECT_NE(orig_ptr, program2.get());
}

} // namespace
