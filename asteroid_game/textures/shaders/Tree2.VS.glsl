#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// Output
out vec2 texture_coord;
out float frag_height;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    texture_coord = v_texture_coord;
    texture_coord.x = atan(v_position.z/v_position.x)/6.28f;
    texture_coord.y = v_position.y / 5.0f;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
