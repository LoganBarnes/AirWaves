// ////////////////////////////////////////////////////////////
// Created on 3/24/18.
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
#include "GLInstance.hpp"
#include <gmock/gmock.h>
#include <gl/Buffer.hpp>
#include <chrono>

namespace {

class BufferTests : public ::testing::Test
{
    gl::testing::GLInstance gl_instance_;
};

TEST_F(BufferTests, BufferIsArrayByDefault)
{
    gl::Buffer<float> buffer = gl::create_buffer(std::vector<float>{});
    EXPECT_EQ(buffer->get_num_elements(), 0);

    EXPECT_EQ(buffer->get_buffer_type(), GL_ARRAY_BUFFER);
}

TEST_F(BufferTests, GetDataReturnsStoredData)
{
    const auto seed = std::chrono::steady_clock::now().time_since_epoch().count();

    std::vector<float> expected(1000000);
    {
        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> dist;
        std::generate(expected.begin(), expected.end(), [&] { return dist(gen); });
    }

    auto buffer = gl::create_buffer(expected);
    EXPECT_EQ(buffer->get_num_elements(), expected.size());

    std::vector<float> actual;
    buffer->get_data(&actual);

    EXPECT_EQ(expected, actual) << "Seed: " << seed;
}

TEST_F(BufferTests, BufferDataCanBeUpdated)
{
    auto buffer = gl::create_buffer(std::vector<float>{0, 1, 2, 3, 4, 5, 6, 7});
    EXPECT_EQ(buffer->get_num_elements(), 8);

    // update values starting at index 2
    buffer->update(2, std::vector<float>{5, 4, 3, 2});
    EXPECT_EQ(buffer->get_num_elements(), 8);

    std::vector<float> expected{0, 1, 5, 4, 3, 2, 6, 7};

    std::vector<float> actual;
    buffer->get_data(&actual);

    EXPECT_EQ(expected, actual);
}

template <typename T>
class TypedBufferTests : public ::testing::Test
{
    gl::testing::GLInstance gl_instance_;
};

typedef ::testing::Types<GLboolean, GLbyte, GLshort, GLint, GLubyte, GLushort, GLuint, GLfloat, GLdouble> GLTypes;
TYPED_TEST_CASE(TypedBufferTests, GLTypes);

TYPED_TEST(TypedBufferTests, NonZeroForAllValidTypes)
{
    auto buffer = gl::create_buffer(std::vector<TypeParam>{});
    EXPECT_NE(buffer->get_data_type(), 0);
}

TEST_F(BufferTests, ZeroForInvalidTypes)
{
    struct CustomType
    {};
    auto buffer = gl::create_buffer(std::vector<CustomType>{});
    EXPECT_EQ(buffer->get_data_type(), 0);
}

} // namespace
