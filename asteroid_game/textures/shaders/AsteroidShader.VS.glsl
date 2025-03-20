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

uniform  vec3 heli_position;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;


// Output
out vec2 texture_coord;
out float frag_height;


void main()
{
    texture_coord = v_texture_coord;
    vec3 new_position = v_position;
    vec4 height = texture(texture_1,texture_coord);
    new_position.y += height.r*24;
    frag_height = height.r*24;
    new_position.y -= pow(length(vec3(heli_position.x,0,heli_position.z)-new_position),2)*0.08;

    gl_Position = Projection * View * Model * vec4(new_position, 1.0);
}
