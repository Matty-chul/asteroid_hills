#version 330

// Input
in vec4 tree_id;

// Uniform properties


// Output
layout(location = 0) out vec3 out_color;
layout(location = 1) out vec4 out_id_tree;


void main()
{
    out_id_tree = tree_id;
}
