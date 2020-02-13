#include "Quad.h"
#include <cstddef>
//static Quad* instance;
//static unsigned int VBO, VAO, EBO;

Quad* Quad::instance = nullptr;
unsigned int Quad::VBO = 0;
unsigned int Quad::VAO = 0;
unsigned int Quad::EBO = 0;

using std::vector;

Quad::Quad() {
	const float sz = 0.5f;
	float quadVertices[] = {
		// positions        	// texture Coords
		 1.0,  0.0,  0.816494,
		 0.0,  1.0,  0.816494,
		-1.0,  0.0,  0.816494,
		 0.0, -1.0,  0.816494,
		 1.0,  0.0, 0.0,
		 0.0,  1.0, 0.0,
		-1.0,  0.0, 0.0,
		 0.0, -1.0, 0.0
	};
	//first face
	//first triangle
	struct Vertex1 aux = { glm::vec3(1.0, 0.0, 0.816494) }; //0
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0,  1.0,  0.816494) }; //1
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0,  0.816494) }; //2
	vertices1.push_back(aux);
	//second triangle
	aux = { glm::vec3(0.0,  1.0,  0.816494) }; //0
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0,  0.816494) }; //2
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0, -1.0,  0.816494) };  //3
	vertices1.push_back(aux);

	//second face
	//first triangle
	aux = { glm::vec3(0.0, -1.0, 0.0) };  //7
	vertices1.push_back(aux);
	aux = { glm::vec3(1.0,  0.0, 0.0) };  //4
	vertices1.push_back(aux);
	aux = { glm::vec3(1.0, 0.0, 0.816494) }; //0
	vertices1.push_back(aux);
	//second triangle
	aux = { glm::vec3(0.0, -1.0, 0.0) };  //7
	vertices1.push_back(aux);
	aux = { glm::vec3(1.0, 0.0, 0.816494) }; //0
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0, -1.0,  0.816494) };  //3
	vertices1.push_back(aux);
	
	//third face
	//first triangle
	aux = { glm::vec3(1.0,  0.0, 0.0) };  //4
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0,  1.0, 0.0) }; //5
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0,  1.0,  0.816494) }; //1
	vertices1.push_back(aux);
	//second triangle
	aux = { glm::vec3(1.0,  0.0, 0.0) };  //4
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0,  1.0,  0.816494) }; //1
	vertices1.push_back(aux);
	aux = { glm::vec3(1.0, 0.0, 0.816494) }; //0
	vertices1.push_back(aux);

	//fourth face
	//first triangle
	aux = { glm::vec3(0.0,  1.0, 0.0) }; //5
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0, 0.0) }; //6
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0,  0.816494) }; //2
	vertices1.push_back(aux);
	//second triangle
	aux = { glm::vec3(0.0,  1.0, 0.0) }; //5
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0,  0.816494) }; //2
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0,  1.0,  0.816494) }; //1
	vertices1.push_back(aux);

	//fifth face
	//first triangle
	aux = { glm::vec3(0.0, -1.0,  0.816494) };  //3
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0,  0.816494) }; //2
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0, 0.0) }; //6
	vertices1.push_back(aux);
	//second triangle
	aux = { glm::vec3(0.0, -1.0,  0.816494) };  //3
	vertices1.push_back(aux);
	aux = { glm::vec3(-1.0,  0.0, 0.0) }; //6
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0, -1.0, 0.0) };  //7
	vertices1.push_back(aux);
	//sixth face
	//first triangle
	aux = { glm::vec3(-1.0,  0.0, 0.0) }; //6
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0,  1.0, 0.0) }; //5
	vertices1.push_back(aux);
	aux = { glm::vec3(1.0,  0.0, 0.0) };  //4
	vertices1.push_back(aux);
	//second triangle
	aux = { glm::vec3(-1.0,  0.0, 0.0) }; //6
	vertices1.push_back(aux);
	aux = { glm::vec3(1.0,  0.0, 0.0) };  //4
	vertices1.push_back(aux);
	aux = { glm::vec3(0.0, -1.0, 0.0) };  //7
	vertices1.push_back(aux);

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2,
	0, 2, 3,
	7, 4, 0, 
	7, 0, 3,
	4, 5, 1,
	4, 5, 0,
	5, 6, 2,
	5, 2, 1,
	3, 2, 6,
	3, 6, 7,
	6, 5, 4,
	6, 4, 7
	};
	for (int i = 0; i < (12*3);i++)
	{
		//std::cout << "q" << indices[i] << " ";
		if ((i+1)% 3 == 0)
		{
			std::cout << '\n';
		}
		indices1.push_back(indices[i]);
	}
	std::cout << "size_indices" << indices1.size() << " ";
	std::cout << "size_vertices" << vertices1.size() << " ";
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(Vertex), &vertices1[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices1.size() * sizeof(unsigned int), &indices1[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (void*)0);

	glBindVertexArray(0);

}

Quad::~Quad() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Quad::Bind()
{
	glBindVertexArray(VAO);
}

void Quad::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices1.size()*vertices1.size());
	//glDrawElements(GL_TRIANGLES, indices1.size()*vertices1.size(), GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	//glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawElements(GL_TRIANGLES, indices1.size(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLE_STRIP, indices1.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
}

Quad* Quad::Instance() {
	if (!instance)
	{
		instance = new Quad();
	}

	return instance;
}
