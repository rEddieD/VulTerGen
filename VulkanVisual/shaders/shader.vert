#version 450

layout (location = 0) in vec3 VertPosition;

void main()
{
	gl_Position = vec4(VertPosition, 1.0);
}
