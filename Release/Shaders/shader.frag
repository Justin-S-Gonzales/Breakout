#version 330

out vec4 color;

uniform vec4 vertColor;

void main()
{
	if (vertColor.x < 0 || vertColor.y < 0 || vertColor.z < 0)
		color = vec4(1.0, 1.0, 1.0, 0.0);
	else
		color = vertColor;
}