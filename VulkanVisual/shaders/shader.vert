#version 450

layout (location = 0) in vec4 VertPosition;

void main()
{
	gl_Position = VertPosition;
}
