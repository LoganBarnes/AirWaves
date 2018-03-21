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
#include "Program.hpp"
#include <unordered_map>
#include <fstream>

namespace gl {

namespace detail {

namespace {

using IdVec = std::vector<std::shared_ptr<GLuint>>;

const std::unordered_map<std::string, GLenum> &shader_types()
{
    static std::unordered_map<std::string, GLenum> ext_map{{".vert", GL_VERTEX_SHADER},
                                                           {".tesc", GL_TESS_CONTROL_SHADER},
                                                           {".tese", GL_TESS_EVALUATION_SHADER},
                                                           {".geom", GL_GEOMETRY_SHADER},
                                                           {".frag", GL_FRAGMENT_SHADER},
                                                           {".comp", GL_COMPUTE_SHADER}};
    return ext_map;
}

const std::unordered_map<GLenum, std::string> &shaderTypeStrings()
{
    static std::unordered_map<GLenum, std::string> type_map{{GL_VERTEX_SHADER, "GL_VERTEX_SHADER"},
                                                            {GL_TESS_CONTROL_SHADER, "GL_TESS_CONTROL_SHADER"},
                                                            {GL_TESS_EVALUATION_SHADER, "GL_TESS_EVALUATION_SHADER"},
                                                            {GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER"},
                                                            {GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER"},
                                                            {GL_COMPUTE_SHADER, "GL_COMPUTE_SHADER"}};
    return type_map;
}

std::string read_file(const std::string &filename)
{
    std::ifstream file(filename, std::ios::in);

    if (!file.is_open() || !file.good()) {
        throw std::runtime_error("Could not read file: " + filename);
    }

    // get full file size
    file.seekg(0, std::ios::end);
    size_t size = static_cast<size_t>(file.tellg());

    // allocate string of correct size
    std::string buffer(size, ' ');

    // fill string with file contents
    // note: string memory is only guaranteed contiguous in C++11 and up
    file.seekg(0);
    file.read(&buffer[0], static_cast<std::streamsize>(size));

    file.close();

    return buffer;
}

std::shared_ptr<GLuint> create_shader(GLenum shader_type, const std::string &filename)
{
    std::shared_ptr<GLuint> shader_id(new GLuint(glCreateShader(shader_type)), [](auto *id) {
        glDeleteShader(*id);
        delete id;
    });

    GLuint shader = *shader_id;

    // Load shader
    std::string shaderStr = read_file(filename);
    const char *shaderSource = shaderStr.c_str();

    // Compile shader
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check shader
    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> shader_error(static_cast<size_t>(log_length));
        glGetShaderInfoLog(shader, log_length, nullptr, shader_error.data());

        // shader will get deleted when shared_ptr goes out of scope
        throw std::runtime_error("(" + shaderTypeStrings().at(shader_type) + ") " + std::string(shader_error.data()));
    }
    return shader_id;
}

std::shared_ptr<GLuint> create_shader(const std::string &filename)
{
    size_t dot = filename.find_last_of(".");

    std::string ext = filename.substr(dot);

    if (shader_types().find(ext) == shader_types().end()) {
        throw std::runtime_error("Unknown shader extension: " + ext);
    }

    return create_shader(shader_types().at(ext), filename);
}

std::shared_ptr<GLuint> create_program(const IdVec &shader_ids)
{
    std::shared_ptr<GLuint> program_id(new GLuint(glCreateProgram()), [shader_ids](auto *id) {
        // TODO: remove this or deleters from shader_ids
        for (auto &shader_id : shader_ids) {
            glDeleteShader(*shader_id);
        }
        glDeleteProgram(*id);
        delete id;
    });

    GLuint program = *program_id;

    for (auto &shader_id : shader_ids) {
        glAttachShader(program, *shader_id);
    }

    glLinkProgram(program);

    // Check program
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result == GL_FALSE) {
        int log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> program_error(static_cast<size_t>(log_length));
        glGetProgramInfoLog(program, log_length, NULL, program_error.data());

        // shaders and programs get deleted when shared_ptr goes out of scope
        throw std::runtime_error("(Program) " + std::string(program_error.data()));
    }

    for (auto &shader_id : shader_ids) {
        glDetachShader(program, *shader_id);
    }

    return program_id;
} // create_program

} // namespace

ProgramWrapper::ProgramWrapper(const std::vector<std::string> &shader_filenames)
{
    IdVec shaders;

    for (const std::string &filename : shader_filenames) {
        shaders.emplace_back(create_shader(filename));
    }

    program_ = create_program(shaders);
}

} // namespace detail

} // namespace gl
