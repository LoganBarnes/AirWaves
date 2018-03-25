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
#include <gl/Texture.hpp>
#include <vmp/VMPConfig.hpp>
#include "GLInstance.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

    EXPECT_EQ(program1->get_id(), program3->get_id());
    EXPECT_EQ(program2->get_id(), program4->get_id());

    EXPECT_NE(program1->get_id(), program2->get_id());
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

TEST_F(ProgramTests, ThrowOnBadFilename)
{
    EXPECT_THROW(gl::create_program("not_a_file.vert"), std::exception);
}

TEST_F(ProgramTests, ThrowOnBadCompilation)
{
    EXPECT_THROW(gl::create_program(vmp::testing::shader_path() + "invalid.vert"), std::exception);
}

TEST_F(ProgramTests, ThrowOnBadLink)
{
    EXPECT_THROW(gl::create_program(vmp::testing::shader_path() + "bad_link.vert",
                                    vmp::testing::shader_path() + "bad_link.frag"),
                 std::exception);
}

TEST_F(ProgramTests, ThrowOnBadExtension)
{
    EXPECT_THROW(gl::create_program(vmp::testing::shader_path() + "bad_extension.glsl"), std::exception);
}

TEST_F(ProgramTests, BoolUniform)
{
    auto program = gl::create_program(vmp::testing::shader_path() + "shader1.vert",
                                      vmp::testing::shader_path() + "uniforms.frag");

    EXPECT_FALSE(program->set_bool_uniform("wrong_name", true));

    EXPECT_TRUE(program->set_bool_uniform("buniform", true));
}

TEST_F(ProgramTests, IntUniform)
{
    auto program = gl::create_program(vmp::testing::shader_path() + "shader1.vert",
                                      vmp::testing::shader_path() + "uniforms.frag");

    glm::ivec4 uniform = {1, 3, 5, 7};
    EXPECT_FALSE(program->set_int_uniform("wrong_name", glm::value_ptr(uniform)));

    EXPECT_TRUE(program->set_int_uniform("iuniform", glm::value_ptr(uniform)));
    EXPECT_TRUE(program->set_int_uniform("i2uniform", glm::value_ptr(uniform), 2));
    EXPECT_TRUE(program->set_int_uniform("i3uniform", glm::value_ptr(uniform), 3));
    EXPECT_TRUE(program->set_int_uniform("i4uniform", glm::value_ptr(uniform), 4));

    EXPECT_THROW(program->set_int_uniform("iuniform", glm::value_ptr(uniform), 0), std::exception);
    EXPECT_THROW(program->set_int_uniform("iuniform", glm::value_ptr(uniform), 5), std::exception);
}

TEST_F(ProgramTests, FloatUniform)
{
    auto program = gl::create_program(vmp::testing::shader_path() + "shader1.vert",
                                      vmp::testing::shader_path() + "uniforms.frag");

    glm::vec4 uniform = {0.1f, 0.3f, 0.5f, 0.7f};
    EXPECT_FALSE(program->set_float_uniform("wrong_name", glm::value_ptr(uniform)));

    EXPECT_TRUE(program->set_float_uniform("funiform", glm::value_ptr(uniform)));
    EXPECT_TRUE(program->set_float_uniform("f2uniform", glm::value_ptr(uniform), 2));
    EXPECT_TRUE(program->set_float_uniform("f3uniform", glm::value_ptr(uniform), 3));
    EXPECT_TRUE(program->set_float_uniform("f4uniform", glm::value_ptr(uniform), 4));

    EXPECT_THROW(program->set_float_uniform("funiform", glm::value_ptr(uniform), 0), std::exception);
    EXPECT_THROW(program->set_float_uniform("funiform", glm::value_ptr(uniform), 5), std::exception);
}

TEST_F(ProgramTests, MatrixUniform)
{
    auto program = gl::create_program(vmp::testing::shader_path() + "shader1.vert",
                                      vmp::testing::shader_path() + "uniforms.frag");

    glm::mat4 uniform(1);
    EXPECT_FALSE(program->set_matrix_uniform("wrong_name", glm::value_ptr(uniform)));

    EXPECT_TRUE(program->set_matrix_uniform("m2uniform", glm::value_ptr(uniform), 2));
    EXPECT_TRUE(program->set_matrix_uniform("m3uniform", glm::value_ptr(uniform), 3));
    EXPECT_TRUE(program->set_matrix_uniform("m4uniform", glm::value_ptr(uniform), 4));

    EXPECT_THROW(program->set_matrix_uniform("muniform", glm::value_ptr(uniform), 1), std::exception);
    EXPECT_THROW(program->set_matrix_uniform("muniform", glm::value_ptr(uniform), 5), std::exception);
}

TEST_F(ProgramTests, TextureUniform)
{
    auto program = gl::create_program(vmp::testing::shader_path() + "shader1.vert",
                                      vmp::testing::shader_path() + "uniforms.frag");

    auto tex = gl::create_texture(128, 128);
    EXPECT_FALSE(program->set_texture_uniform("wrong_name", tex));

    EXPECT_TRUE(program->set_texture_uniform("tex", tex));
}

} // namespace
