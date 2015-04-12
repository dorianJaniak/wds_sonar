#version 150

uniform mat4 Mmove;
uniform mat4 Mproj;

in vec4 vVertex;


void main()
{
	gl_Position = Mproj * Mmove * vVertex;
}