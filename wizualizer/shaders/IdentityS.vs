#version 150

uniform mat4 MVP;

in vec4 vVertex;


void main()
{
	vec4 vertex = MVP * vVertex;
	gl_Position.xyz = vertex.xyz;
}