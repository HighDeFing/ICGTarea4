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
		 min[0],  min[1],  min[2],
		 min[0],  min[1],  max[2],
		 min[0],  max[1],  min[2],
		// min[0],  max[1],  max[2],
		// max[0],  min[1],  min[2],
		// max[0],  min[1],  max[2],
		// max[0],  max[2],  min[2],
		// max[0],  max[2],  max[2]
	};

	//unsigned int indices[] = {  // note that we start from 0!
	//0, 1, 2,
	///*0, 2, 3,
	//7, 4, 0, 
	//7, 0, 3,
	//4, 5, 1,
	//4, 5, 0,
	//5, 6, 2,
	//5, 2, 1,
	//3, 2, 6,
	//3, 6, 7,
	//6, 5, 4,
	//6, 4, 7*/
	//};

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
}

Quad* Quad::Instance() {
	if (!instance)
	{
		instance = new Quad();
	}

	return instance;
}
