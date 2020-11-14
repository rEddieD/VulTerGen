#version 450

layout (location = 0) in vec3 VertPosition;
layout (location = 0) out vec3 FragColor;

layout( push_constant ) uniform pushConstants
{
	vec4 color;
} pushConstant;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
	mat4 proj;
} ubo;

vec3 colors[4] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0) + pushConstant.color.rgb,
    pushConstant.color.rgb
);

void main()
{
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(VertPosition, 1.0);
    FragColor = colors[gl_VertexIndex];
}
