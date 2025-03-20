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
    // TODO(student): Calculate the out_color using the texture() function.
    vec4 leaf = texture (texture_3, texture_coord);
    
    out_color = leaf;
}
