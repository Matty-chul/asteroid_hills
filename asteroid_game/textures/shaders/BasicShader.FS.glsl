#version 410
// sau version 410?
// Input
// TODO(student): Get values from vertex shader
in vec3 color;
// Output
layout(location = 0) out vec4 out_color;



void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(color,1);


}
