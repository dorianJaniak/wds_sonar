#version 110

uniform mat4 MVP;

in vec4 vVertex;


void main()
{
	gl_Position = MVP * vVertex;
}