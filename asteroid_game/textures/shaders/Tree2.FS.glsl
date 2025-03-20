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
    vec4 color = vec4(1,0,0,0);
    vec4 bark = texture (texture_2, texture_coord);
    vec4 leaf = texture (texture_3, texture_coord);
    

    color = mix (bark, leaf, 0.5);
    out_color = color;
}
