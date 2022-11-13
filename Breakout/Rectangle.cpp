#include "Rectangle.h"

Rectangle::Rectangle()
{
	// Mesh
	unsigned int numOfVertices = 16;
	float vertices[] = {
		-0.5f, 0.5f,   // Top left			1		0
		0.5f, 0.5f,    // Top right			2		1
		0.5f, -0.5f,   // Bottom right		3		2
		-0.5f, -0.5f   // Bottom left		4		3
	};

	unsigned int numOfIndices = 6;
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	CreateMesh(indices, numOfIndices, vertices, numOfVertices);
}
