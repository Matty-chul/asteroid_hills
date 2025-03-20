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



void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    texture_coord = v_texture_coord;
    texture_coord.x = atan(v_position.z,v_position.x+0.00001)/6.28f;
    texture_coord.y = v_position.y / 5.0f;

    vec4 world_position = Model*vec4(v_position,1.0);
    vec4 height = texture(texture_1,vec2((world_position.x+10.0f)/20,(world_position.z+10.0f)/20)); 
    world_position.y += height.r*24;//treescale;
    world_position.y -= pow(length(heli_position-vec3(world_position)),2)*0.08;

    gl_Position = Projection * View * world_position;
}
