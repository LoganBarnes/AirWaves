// ////////////////////////////////////////////////////////////
// Created on 3/21/18.
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
#version 410
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 world_position;
layout(location = 1) in vec2 tex_coords;

uniform mat4 screen_from_world = mat4(1);

out Vertex {
    vec2 tex_coords;
} vertex;

out gl_PerVertex
{
  vec4 gl_Position;
};

void main()
{
    vertex.tex_coords = tex_coords;
	gl_Position = screen_from_world * vec4(world_position, 1.0);
}
