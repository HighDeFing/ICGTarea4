#pragma once


#ifndef __ED_QUAD__
#define __ED_QUAD__


#include <glad/glad.h>
#include "glm/gtx/string_cast.hpp"
#include "EDpch.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "Mesh.h"

using std::vector;
extern struct Vertex;

class Quad {
	static Quad* instance;
	unsigned int VBO, VAO, EBO;
	unsigned int texture;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	Quad();
	~Quad();
public:
	glm::vec3 min, max;
	void Draw();
	void Bind();
	static Quad* Instance();
	void CreateTexture();
	void SetMesh();
	void BindTexture();
};
#endif // !__ED_QUAD__


