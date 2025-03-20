#version 330

// Input
in vec2 texture_coord;
in float frag_height;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;


// Output
layout(location = 0) out vec4 out_color;


void main()
{

    vec4 bark = texture (texture_2, texture_coord);

    out_color = bark;
}
