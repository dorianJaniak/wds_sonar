#version 150

in float VertexZ;
out vec4 vFragColor;
uniform vec4 materialColor;

void main()
{
	float subV = VertexZ;
	if(subV > 0.5f)
		subV = 0.5f;
	vFragColor = materialColor + vec4(subV*0.2,subV*0.2,subV*0.2,0.0f);
}