#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
};

class Mesh
{
public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh();
	~Mesh();

	void SetUpMesh();
	void Draw();
};

#endif
