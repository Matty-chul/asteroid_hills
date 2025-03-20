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

uniform float object_id;

// Output
out vec2 texture_coord;
out float frag_height;
out vec4 tree_id;

void main()
{
    texture_coord = v_texture_coord;
    vec4 world_position = Model * vec4(v_position, 1.0f);
    vec4 height = texture(texture_1,vec2((world_position.x+10.0f)/20,(world_position.z+10.0f)/20));
    world_position.y += height.r*24;

    tree_id = vec4(vec3(object_id/10),object_id);

    world_position.y -= pow(length(vec3(heli_position.x,0,heli_position.z)-vec3(world_position)),2)*0.08;

    gl_Position = Projection * View * world_position;
}
