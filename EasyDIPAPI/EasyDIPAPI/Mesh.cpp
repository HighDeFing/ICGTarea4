
#include "Mesh.h"
#include <iostream>
#include <filesystem>

#include "stb_image.h"
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


void Mesh::MeshCreate(vector<Vertex> vertices, vector<unsigned int> indices) {

	this->vertices = vertices;
	this->indices = indices;
	setupMesh();
	loadCreateTexture("notihnyet");
	
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);
}

void Mesh::BindTexture()
{
	//bwShader->setInt("ourTexture", 0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindVertexArray(VAO);
}

void Mesh::loadCreateTexture(std::string path)
{
	// texture 1
	// ---------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("C:/Users/heide/Desktop/ICG/[ICG] Tarea #4 - 24981800/ICGTarea4/texture/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	bwShader->setInt("ourTexture", 0);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::Bind()
{
	glBindVertexArray(VAO);
}

void Mesh::Draw()
{
	bwShader->setMat4("mModel", modelMatrix);
	bwShader->setMat4("mView", view);
	bwShader->setMat4("mProj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (zbuffer) {
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else{
		glDisable(GL_DEPTH_TEST);
	}
	if(back_face_culling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
	if (relleno)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		bwShader->setVec4("my_color", colorrelleno);
		glPolygonOffset(8.0f, 8.0f);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	if (mallado)
	{
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		bwShader->setVec4("my_color", colormesh);
		glPolygonOffset(4.0f, 4.0f);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
	if (points)
	{
		glPointSize(1.5f);
		bwShader->setVec4("my_color", colorpoints);
		glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(VAO);
	glBindVertexArray(0);
}

void Mesh::DrawNormals()
{
	glDrawElements(GL_LINE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::create_BoundingBox(glm::vec3 min, glm::vec3 max)
{
	float quadVertices[] = {
		// positions        	// texture Coords
		 min[0],  min[1],  min[2],
		 min[0],  min[1],  min[2],
		 min[0],  max[1],  min[2],
		 min[0],  max[1],  max[2],
		 max[0],  min[1],  min[2],
		 max[0],  min[1],  max[2],
		 max[0],  max[2],  min[2],
		 max[0],  max[2],  max[2]
	};
}

void Mesh::setView(glm::mat4 input)
{
	view = input;
}

void Mesh::setmodelMatrix(glm::mat4 input)
{
	modelMatrix = input;
}

void Mesh::setproj(glm::mat4 input)
{
	proj = input;
}

//Mesh* Mesh::Instance() {
//	if (!instance)
//	{
//		instance = new Mesh();
//	}
//
//	return instance;
//	return new Mesh();
//}

//void Mesh::set_triangles(vector<CG::triangle> trian)
//{
//	triangles = trian;
//}
