
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
	//loadCreateTexture("notihnyet");
	
}

void Mesh::setupMesh()
{
	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	std::cout << i << '\n';
	//	std::cout << "Position:" << glm::to_string(vertices[i].Position) << '\n';
	//	//std::cout << "Normals:" << glm::to_string(vertices[i].Normal) << '\n';
	//	std::cout << "Textures:" << glm::to_string(vertices[i].TexCoords) << '\n';
	//}
	//vertices.clear();
	//std::cout << "Con los de mesh" << '\n' << '\n';
	//Vertex aux; aux.Position = { 0.5f,  0.5f, 0.0f };
	//vertices.insert(vertices.begin() + 0, aux);
	//vertices.insert(vertices.begin() + 1, aux);
	//vertices.insert(vertices.begin() + 2, aux);
	//vertices.insert(vertices.begin() + 3, aux);
	//vertices[0].Position = { 1.0f,  1.0f, 0.0f }; vertices[0].Normal = { 1.0f, 0.0f, 0.0f }; vertices[0].TexCoords = { 1.0f, 1.0f };
	//vertices[1].Position = { 1.0f, -1.0f, 0.0f }; vertices[1].Normal = { 0.0f, 1.0f, 0.0f }; vertices[1].TexCoords = { 1.0f, 0.0f };
	//vertices[2].Position = { -1.0f, -1.0f, 0.0f }; vertices[2].Normal = { 0.0f, 0.0f, 1.0f }; vertices[2].TexCoords = { 0.0f, 0.0f };
	//vertices[3].Position = { -1.0f,  1.0f, 0.0f }; vertices[3].Normal = { 1.0f, 1.0f, 0.0f }; vertices[3].TexCoords = { 0.0f, 1.0f };
	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	std::cout << i << '\n';
	//	std::cout << "Position:" << glm::to_string(vertices[i].Position) << '\n';
	//	//std::cout << "Normals:" << glm::to_string(vertices[i].Normal) << '\n';
	//	std::cout << "Textures:" << glm::to_string(vertices[i].TexCoords) << '\n';
	//}

	//for (int i = 0; i < indices.size(); i++)
	//{
	//	std::cout<<indices[i];
	//}
	vec4ftraslate[0] = vec4ftraslate[1] = vec4ftraslate[2] = 0.0f;
	vec4fscale[0] = vec4fscale[1] = vec4fscale[2] = vec4fscale[3] = 1.0f;
	Qrotacion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

}

void Mesh::BindTexture()
{
	//bwShader->setInt("ourTexture", 0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Mesh::loadCreateTexture(const char* path)
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
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
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
	glDeleteBuffers(1, &EBO);
}

void Mesh::Bind()
{
	glBindVertexArray(VAO);
}


void Mesh::Draw()
{   //model shader
	bwShader->setMat4("mModel", modelMatrix);
	bwShader->setMat4("mView", view);
	bwShader->setMat4("mProj", proj);
	bwShader->setMat3("mTransposeModel", transpose(inverse(modelMatrix)));
	//texture shaders
	bwShader->setBool("texture_drawing", texture_drawing);
	bwShader->setBool("texture_with_color", texture_with_color);
	bwShader->setBool("only_color", only_color);
	bwShader->setBool("only_texture", only_texture);
	//light shader
	//if (zbuffer) {
	//	glEnable(GL_DEPTH_TEST);
	//	//glDepthMask(GL_FALSE);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//}
	//else{
	//	glDisable(GL_DEPTH_TEST);
	//}
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
		bwShader->setVec3("objectColor", glm::vec3(colorrelleno[0], colorrelleno[1], colorrelleno[2]));
		glPolygonOffset(8.0f, 8.0f);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	if (mallado)
	{
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		bwShader->setVec4("my_color", colormesh);
		bwShader->setVec3("objectColor", glm::vec3(colormesh[0], colormesh[1], colormesh[2]));
		glPolygonOffset(4.0f, 4.0f);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
	if (points)
	{
		glPointSize(1.5f);
		bwShader->setVec4("my_color", colorpoints);
		bwShader->setVec3("objectColor", glm::vec3(colorpoints[0], colorpoints[1], colorpoints[2]));
		glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	}
	//glBindVertexArray(VAO);
	//glBindVertexArray(0);
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

void Mesh::setmodelMatrix()
{
	glm::mat4 R = mat4_cast(Qrotacion);
	glm::mat4 S;
	if (vec4fscale.w != 1.0f)
	{
		S = glm::scale(glm::vec3(vec4fscale.w, vec4fscale.w, vec4fscale.w));
	}
	else {
		S = glm::scale(glm::vec3(vec4fscale.x, vec4fscale.y, vec4fscale.z));
	}

	glm::mat4 T = glm::translate(vec4ftraslate);
	modelMatrix = T * S * R;

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
