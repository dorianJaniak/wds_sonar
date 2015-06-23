#version 150

uniform mat4 Mmove;
uniform mat4 Mproj;

in vec4 vVertex;
out float VertexZ;


void main()
{
	gl_Position = Mproj * Mmove * vVertex;
	VertexZ = gl_Position.z - 1.0f;
}