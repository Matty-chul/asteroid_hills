#version 330

// Input
in vec4 dir_vector;

// Uniform properties


// Output
layout(location = 0) out vec3 out_color;
layout(location = 1) out vec4 out_dir;


void main()
{
    out_dir = dir_vector;
}
