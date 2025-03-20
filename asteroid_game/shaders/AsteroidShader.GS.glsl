#version 330

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;

// Input
in vec2 texture_coord;
in float frag_height;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;

uniform float time;

// Output
out vec3 color;



void main()
{
	for (int i=0; i<3;i++){
		color = vec3(1,0,0);
		gl_Position = Projection * View * vec4 (gl_in[i].gl_Position.xyz, 1);
		EmitVertex();
	}
	EndPrimitive();
}
