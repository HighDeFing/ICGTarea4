#pragma once

#ifndef __ED_MESH__
#define __ED_MESH__

#include <glad/glad.h>
#include "glm/gtx/string_cast.hpp"
#include "EDpch.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <cstddef>
#include "shaders/Shader.h"

using std::vector;
extern Shader *bwShader;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
};


class Mesh {
	unsigned int VBO, VAO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	~Mesh();
public:
	bool mallado;
	bool points;
	bool back_face_culling;
	bool zbuffer;
	bool relleno;
	glm::vec4 colorrelleno = { 0.4f,0.7f,0.0f,0.5f };
	glm::vec4 colormesh = { 1.0f,1.0f,1.0f,1.0f };
	glm::vec4 colorpoints = { 0.0f,0.0f,0.0f,0.0f};
	glm::mat4 modelMatrix;
	glm::mat4 proj;
	glm::quat Qrotacion;

	float vec4fscale[4]= { 0.5f, 0.5f, 0.5f, 0.5f };
	float vec4ftraslate[4];

	void MeshCreate(vector<Vertex> vertices, vector<unsigned int> indices);
	void Draw();
	void Bind();
	void setupMesh();
	void DrawNormals();
	void create_BoundingBox(glm::vec3 min, glm::vec3 max);
	void setmodelMatrix(glm::mat4);
	void setproj(glm::mat4);
	//static Mesh* Instance();
};
#endif // !__ED_MESH__
