#include "lab_m1/TankWars/objects.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


using namespace tw;

Mesh* objects::CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill)
{
	float halfLength = length / 2.0;

	glm::vec3 bottomLeft = glm::vec3(-halfLength, -halfLength, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(bottomLeft, color),
		VertexFormat(bottomLeft + glm::vec3(length, 0, 0), color),
		VertexFormat(bottomLeft + glm::vec3(length, length, 0), color),
		VertexFormat(bottomLeft + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// Draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}
