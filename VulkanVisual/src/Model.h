#pragma once
#include <vulkan/vulkan.hpp>

class Model
{
	uint32_t size;
	float* VertexArray;
	bool onHeap;

public:
	Model(const float* vertexArray, size_t size); //(const float vertexArray[], uin32_t size)
	Model(float vertexArray[], size_t size);
	~Model();

	void PrintEach();
};