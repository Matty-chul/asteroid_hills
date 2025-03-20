#version 410

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
//layout(location = 1) in vec3 v_normal;
//layout(location = 2) in vec2 v_tex_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float ElapsedTime;
uniform vec3 object_color;
uniform  vec3 heli_position;

uniform sampler2D texture_1;


// Output
// TODO(student): Output values to fragment shader
out vec3 color;
out vec2 world_texture_coords;
out vec4 heli_height;

void main()
{
    vec4 world_position = Model * vec4(v_position,1.0f);
    world_texture_coords = vec2((heli_position.x + 10.0f)/20,(heli_position.z +10.0f)/20);

    heli_height = texture(texture_1, world_texture_coords);
    world_position.y += heli_height.r*24;
    //world_position.y -= pow(length(heli_position),2)*0.005;

    color = object_color;

    gl_Position = Projection * View * world_position;
}