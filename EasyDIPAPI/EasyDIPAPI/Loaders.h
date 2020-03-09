#include "EDpch.h"
#include "Mesh.h"

namespace CG
{
	class triangle {
	public:
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 normal;
	};
	Mesh* Load(const std::string path);
	static Mesh* LoadObj(const std::string path);
	static Mesh* LoadOff(const std::string path);
}