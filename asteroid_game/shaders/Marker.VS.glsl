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
uniform vec3 object_color;

uniform sampler2D texture_1;


// Output
out vec3 color;



void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader

    vec4 world_position = Model*vec4(v_position,1.0);
    vec4 height = texture(texture_1,vec2((world_position.x+10.0f)/20,(world_position.z+10.0f)/20)); 
    world_position.y += height.r*24;
    world_position.y -= pow(length(heli_position-vec3(world_position)),2)*0.08;

    color = object_color;
    gl_Position = Projection * View * world_position;
}
