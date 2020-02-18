#include "EDpch.h"


namespace CG
{
	class triangle {
	public:
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 normal;
	};
	void Load(const std::string path);
	static void LoadObj(const std::string path);
	static void LoadOff(const std::string path);
}