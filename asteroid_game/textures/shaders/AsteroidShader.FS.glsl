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
    
    vec4 rock = texture (texture_2, texture_coord);
    vec4 rock2 = texture (texture_3, texture_coord);


    if (frag_height<=0.6) color = rock;
    if (frag_height >0.6 && frag_height <= 1.1 ) color = mix(rock, rock2, (frag_height-0.6f)/0.5f);
    if (frag_height > 1.1 )color= rock2;
    //color = 

    out_color  = color;
}
