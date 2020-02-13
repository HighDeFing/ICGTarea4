#pragma once

#ifndef __ED_MESH__
#define __ED_MESH__

#include <glad/glad.h>
#include "glm/gtx/string_cast.hpp"
#include "EDpch.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vector>

using std::vector;

struct Vertex {
	glm::vec3 Position;
};

class Mesh {
	static Mesh* instance;
	static unsigned int VBO, VAO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	~Mesh();
public:
	void MeshCreate(vector<Vertex> vertices, vector<unsigned int> indices);
	void Draw();
	void Bind();
	void setupMesh();
	static Mesh* Instance();
};
#endif // !__ED_MESH__
