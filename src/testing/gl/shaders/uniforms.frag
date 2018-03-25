// ////////////////////////////////////////////////////////////
// Created on 3/25/18.
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
#version 120

uniform bool buniform;

uniform int iuniform;
uniform ivec2 i2uniform;
uniform ivec3 i3uniform;
uniform ivec4 i4uniform;

uniform float funiform;
uniform vec2 f2uniform;
uniform vec3 f3uniform;
uniform vec4 f4uniform;

uniform mat2 m2uniform;
uniform mat3 m3uniform;
uniform mat4 m4uniform;

uniform sampler2D tex;

void main()
{
    vec4 color;

    if (buniform) {
        color = vec4(0.1, 0.2, 0.3, 0.4);
    } else {
        color = texture2D(tex, f2uniform);
    }

    color.x += iuniform;
    color.xy += i2uniform;
    color.xyz += i3uniform;
    color.xyzw += i4uniform;

    color.r *= funiform;
    color.rg *= f2uniform;
    color.rgb *= f3uniform;
    color.rgba *= f4uniform;

    color.st = m2uniform * color.st;
    color.stp = m3uniform * color.stp;
    color.stpq = m4uniform * color.stpq;

	gl_FragColor = color;
}
