
#include "Light.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

//static Quad* instance;
//static unsigned int VBO, VAO, EBO;

//Mesh* Mesh::instance = nullptr;
//unsigned int Mesh::VBO = 0;
//unsigned int Mesh::VAO = 0;
//unsigned int Mesh::EBO = 0;

// C:/Users/heide/Desktop/ICG/[ICG] Tarea #4 - 24981800/ICGTarea4/EasyDIPAPI/EasyDIPAPI/shaders/shader.vert
// C:/Users/heide/Desktop/ICG/[ICG] Tarea #4 - 24981800/ICGTarea4/EasyDIPAPI/EasyDIPAPI/shaders/shader.frag
// C:/Users/heide/Desktop/ICG/[ICG] Tarea #4 - 24981800/ICGTarea4/EasyDIPAPI/EasyDIPAPI/shaders/shader.geom


void Light::setupMesh()
{
	 float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	 
	 glGenBuffers(1, &VBO);

	 glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	 glGenVertexArrays(1, &lightVAO);
	 glBindVertexArray(lightVAO);
	 //glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 // note that we update the lamp's position attribute's stride to reflect the updated buffer data
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	 glEnableVertexAttribArray(0);
	 //glm::vec3 lightPos(0.9f, 0.9f, 1.0f);
	 //modelMatrix = glm::mat4(1.0f);
	 //modelMatrix = glm::translate(modelMatrix, lightPos);
	 //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // a smaller cube

}

Light::~Light() {
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
}

void Light::Bind()
{
	glBindVertexArray(lightVAO);
}

void Light::Connect_shader()
{
	bwShader->setFloat("intensity_ambiental", intensity_ambiental);
	bwShader->setFloat("intensity_specular", intensity_specular);
	bwShader->setBool("bambient", bambient);
	bwShader->setBool("bdiffuse", bdiffuse);
	bwShader->setBool("bspecular", bspecular);
	bwShader->setVec3("lightColor", glm::vec3(colorrelleno[0], colorrelleno[1], colorrelleno[2]));
	bwShader->setVec3("lightPos", glm::vec3(vec4ftraslate[0], vec4ftraslate[1], vec4ftraslate[2]));

}


void Light::Draw()
{
	lampShader->setMat4("mModel", modelMatrix);
	lampShader->setMat4("mView", view);
	lampShader->setMat4("mProj", proj);
	lampShader->setVec4("my_color", colorrelleno);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glBindVertexArray(lightVAO);
	//glBindVertexArray(0);
}


void Light::setView(glm::mat4 input)
{
	view = input;
}

void Light::setmodelMatrix(glm::mat4 input)
{
	modelMatrix = input;
}

void Light::setproj(glm::mat4 input)
{
	proj = input;
}

