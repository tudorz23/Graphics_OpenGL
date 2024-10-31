#include "lab_m1/TankWars/objects.h"

#include "lab_m1/TankWars/constants.h"
#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


using namespace tw;

Mesh* objects::CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill)
{
	float halfLength = length / 2.0f;

	glm::vec3 bottomLeft = glm::vec3(-halfLength, -halfLength, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(bottomLeft, color),
		VertexFormat(bottomLeft + glm::vec3(length, 0, 0), color),
		VertexFormat(bottomLeft + glm::vec3(length, length, 0), color),
		VertexFormat(bottomLeft + glm::vec3(0, length, 0), color)
	};

	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

	Mesh* square = new Mesh(name);

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


Mesh* objects::CreateTerrain(const std::string& name, glm::vec3 color,
							 std::vector<std::pair<float, float>> heightMap)
{
	std::vector<VertexFormat> vertices;

	for (const auto& point : heightMap) {
		vertices.push_back(VertexFormat(glm::vec3(point.first, point.second, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(point.first, 0, 0), color));
	}

	std::vector<unsigned int> indices;
	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	Mesh* terrain = new Mesh(name);
	terrain->SetDrawMode(GL_TRIANGLE_STRIP);
	terrain->InitFromData(vertices, indices);
	return terrain;
}



/**
*		*************
*      **			**
*	  *****************
*/
Mesh* objects::CreateTrapezoid(const std::string& name, float length, float height,
						glm::vec3 color)
{
	// Center will be (0, 0).
	glm::vec3 center = glm::vec3(0, 0, 0);
	glm::vec3 rectangleBottLeft = center - glm::vec3(length / 2.0, height / 2.0, 0);

	float triangleEdge = 0.66f * height;

	std::vector<VertexFormat> vertices =
	{
		// Left triangle.
		VertexFormat(rectangleBottLeft - glm::vec3(triangleEdge, 0, 0), color),
		VertexFormat(rectangleBottLeft, color),
		VertexFormat(rectangleBottLeft + glm::vec3(0, height, 0), color),

		// Rectangle.
		VertexFormat(rectangleBottLeft + glm::vec3(length, height, 0), color),
		VertexFormat(rectangleBottLeft + glm::vec3(length, 0, 0), color),

		// Right triangle.
		VertexFormat(rectangleBottLeft + glm::vec3(length + triangleEdge, 0, 0), color)
	};

	std::vector<unsigned int> indices = { 0, 1, 2,
										  1, 3, 2,
										  1, 4, 3,
										  4, 5, 3 };

	Mesh* trapezoid = new Mesh(name);
	trapezoid->InitFromData(vertices, indices);

	return trapezoid;
}


Mesh* objects::CreateSemiCircle(const std::string& name, float radius, glm::vec3 color)
{
	glm::vec3 center = glm::vec3(0, 0, 0);

	std::vector<VertexFormat> vertices { VertexFormat(center, color) };
	std::vector<unsigned int> indices;

	float angle_base = M_PI / (float) (NUM_TRIANGLES - 1);

	for (int i = 0; i < NUM_TRIANGLES; i++) {
		float angle = angle_base * i;

		float x = radius * glm::cos(angle);
		float y = radius * glm::sin(angle);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		indices.push_back(i);
	}

	indices.push_back(NUM_TRIANGLES);

	Mesh* semicircle = new Mesh(name);

	semicircle->SetDrawMode(GL_TRIANGLE_FAN);
	semicircle->InitFromData(vertices, indices);
	return semicircle;
}
