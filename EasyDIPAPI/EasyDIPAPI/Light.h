#pragma once

#ifndef __ED_LIGHT__
#define __ED_LIGHT__

#include <glad/glad.h>
#include "glm/gtx/string_cast.hpp"
#include "EDpch.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <filesystem> 
#include <vector>
#include <cstddef>
#include "shaders/Shader.h"

using std::vector;
extern Shader* bwShader; //shader of objects
extern Shader* lampShader;

//struct Vertex {
//	glm::vec3 Position;
//	glm::vec3 Normal;
//	glm::vec2 TexCoords;
//};


class Light {
	unsigned int VBO, lightVAO;

	~Light();
public:
	bool relleno = true;
	bool bambient = true;
	bool bdiffuse = false;
	bool bspecular = false;
	float intensity_ambiental;
	float intensity_specular;
	glm::vec4 colorrelleno = glm::vec4(1.0f);
	glm::vec4 colormesh = glm::vec4(1.0f);
	glm::vec4 colorpoints = glm::vec4(1.0f);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 proj;
	glm::mat4 view;

	float vec4fscale[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	float vec4ftraslate[4];

	void Connect_shader();
	void Draw();
	void Bind();
	void setupMesh();
	void setmodelMatrix(glm::mat4);
	void setproj(glm::mat4);
	void setView(glm::mat4);
};
#endif // !__ED_LIGHT__
