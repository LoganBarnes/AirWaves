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

in Vertex {
    vec2 tex_coords;
} vertex;

uniform bool use_tex = false;
uniform sampler2D tex;

layout(location = 0) out vec4 out_color;

void main()
{
    if (use_tex) {
        out_color = texture(tex, vertex.tex_coords);
    } else {
        out_color = vec4(vertex.tex_coords, 1.0, 1.0);
    }
}
