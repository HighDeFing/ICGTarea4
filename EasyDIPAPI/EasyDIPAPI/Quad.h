#pragma once


#ifndef __ED_QUAD__
#define __ED_QUAD__


#include <glad/glad.h>
#include "glm/gtx/string_cast.hpp"
#include "EDpch.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vector>

using std::vector;

struct Vertex1 {
	glm::vec3 Position;
};

class Quad {
	static Quad* instance;
	static unsigned int VBO, VAO, EBO;
	vector<Vertex1> vertices1;
	vector<unsigned int> indices1;
	Quad();
	~Quad();
public:
	void Draw();
	void Bind();
	static Quad* Instance();
};
#endif // !__ED_QUAD__


