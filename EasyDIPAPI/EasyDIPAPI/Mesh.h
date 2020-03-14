#pragma once

#ifndef __ED_MESH__
#define __ED_MESH__

#include <glad/glad.h>
#include "glm/gtx/string_cast.hpp"
#include "EDpch.h"
#include <glm/vec3.hpp>
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/glm.hpp>
#include <filesystem> 
#include <vector>
#include <cstddef>
#include "shaders/Shader.h"

using std::vector;
extern Shader *bwShader; //shader of objects

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


class Mesh {
	unsigned int VBO, VAO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	unsigned int texture;

	~Mesh();
public:
	bool mallado = true;
	bool points;
	bool back_face_culling;
	bool zbuffer;
	bool relleno;
	bool texture_drawing = false;
	bool texture_with_color;
	bool only_color = true;
	bool only_texture;
	int texture_mode = 0; 

	glm::vec4 colorrelleno = { 0.4f,0.7f,0.0f,0.5f };
	glm::vec4 colormesh = { 1.0f,1.0f,1.0f,1.0f };
	glm::vec4 colorpoints = { 0.0f,0.0f,0.0f,0.0f };
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 proj;
	glm::mat4 view;
	glm::quat Qrotacion;

	glm::vec4 vec4fscale;
	glm::vec3 vec4ftraslate;

	void MeshCreate(vector<Vertex> vertices, vector<unsigned int> indices);
	void Draw();
	void Bind();
	void setupMesh();
	void DrawNormals();
	void create_BoundingBox(glm::vec3 min, glm::vec3 max);
	void setmodelMatrix();
	void setproj(glm::mat4);
	void setView(glm::mat4);
	void loadCreateTexture(const char*);
	void BindTexture();
	//static Mesh* Instance();
};
#endif // !__ED_MESH__
