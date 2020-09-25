#include "Model.h"
#include <iostream>

Model::Model(const float* vertexArray, size_t size)
{
	this->size = size;
	this->VertexArray = new float[this->size];
	this->onHeap = true;
	memcpy(VertexArray, vertexArray, size);
}

Model::Model(float vertexArray[], size_t size)
{
	this->size = size;
	this->VertexArray = vertexArray;
	this->onHeap = false;
	std::cout << "" << std::endl;
}

Model::~Model()
{
	if ((this->VertexArray != nullptr) && (this->onHeap == true))
		delete[] VertexArray;
}

void Model::PrintEach()
{
	for (int i = 0; i < size; ++i)
	{
		std::cout << *(VertexArray + i) << std::endl;
	}
}
