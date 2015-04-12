#version 150

out vec4 vFragColor;
uniform vec4 materialColor;

void main()
{
	vFragColor = materialColor;
}