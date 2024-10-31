#include "lab_m1/TankWars/objects.h"

#include "lab_m1/TankWars/constants.h"
#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include <iostream>


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

	float angle_base = M_PI / (float) (NUM_TRIANGLES_SEMICIRCLE - 1);

	for (int i = 0; i < NUM_TRIANGLES_SEMICIRCLE; i++) {
		float angle = angle_base * i;

		float x = radius * glm::cos(angle);
		float y = radius * glm::sin(angle);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		indices.push_back(i);
	}

	indices.push_back(NUM_TRIANGLES_SEMICIRCLE);

	Mesh* semicircle = new Mesh(name);

	semicircle->SetDrawMode(GL_TRIANGLE_FAN);
	semicircle->InitFromData(vertices, indices);
	return semicircle;
}


Mesh* objects::CreateTank(const std::string& name, glm::vec3 color1, glm::vec3 color2)
{
	// Lower trapezoid.
	glm::vec3 lowerBottomLeft = glm::vec3(-TANK_LOWER_LENGTH / 2.f, 0, 0);
	glm::vec3 lowerTopLeft = lowerBottomLeft + glm::vec3(-TANK_LOWER_TR_LEN, TANK_LOWER_HEIGHT, 0);
	glm::vec3 lowerTopRight = lowerBottomLeft + glm::vec3(TANK_LOWER_LENGTH + TANK_LOWER_TR_LEN, TANK_LOWER_HEIGHT, 0);
	glm::vec3 lowerBottomRight = lowerBottomLeft + glm::vec3(TANK_LOWER_LENGTH, 0, 0);

	// Upper trapezoid.
	glm::vec3 upperTopLeft = glm::vec3(-TANK_UPPER_LENGTH / 2.f, TANK_LOWER_HEIGHT + TANK_UPPER_HEIGHT, 0);
	glm::vec3 upperTopRight = upperTopLeft + glm::vec3(TANK_UPPER_LENGTH, 0, 0);
	glm::vec3 upperBottomRight = upperTopRight + glm::vec3(TANK_UPPER_TR_LEN, -TANK_UPPER_HEIGHT, 0);
	glm::vec3 upperBottomLeft = upperTopLeft + glm::vec3(-TANK_UPPER_TR_LEN, -TANK_UPPER_HEIGHT, 0);

	// Semicircle center.
	glm::vec3 center = glm::vec3(0, TANK_LOWER_HEIGHT + TANK_UPPER_HEIGHT, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(lowerBottomLeft, color1),
		VertexFormat(lowerTopLeft, color1),
		VertexFormat(lowerTopRight, color1),
		VertexFormat(lowerBottomRight, color1),

		VertexFormat(upperTopLeft, color2),
		VertexFormat(upperTopRight, color2),
		VertexFormat(upperBottomRight, color2),
		VertexFormat(upperBottomLeft, color2),

		VertexFormat(center, color2)
	};

	std::vector<unsigned int> indices = { 0, 2, 1, 0, 3, 2,
										  7, 5, 4, 7, 6, 5 };

	// Semicircle other points.
	float angle_base = M_PI / (float)(NUM_TRIANGLES_SEMICIRCLE - 1);

	// Point of angle 0 (vertex of index 9).
	glm::vec3 angle0 = center + glm::vec3(-TANK_CAP_RADIUS, 0, 0);
	vertices.push_back(VertexFormat(angle0, color2));

	for (int i = 1; i < NUM_TRIANGLES_SEMICIRCLE; i++) {
		float angle = angle_base * i;

		if (angle < M_PI / 2.0) {
			angle = M_PI - angle;
		}

		float x = center.x + TANK_CAP_RADIUS * glm::cos(angle);
		float y = center.y + TANK_CAP_RADIUS * glm::sin(angle);

		// Vertex index 9 + i.
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color2));
		indices.push_back(8);
		indices.push_back(9 + i - 1);
		indices.push_back(9 + i);

		std::cout << 8 << " " << 9 + i - 1 << " " << 9 + i << "\n";
		std::cout << x << " " << y << "\n";
		std::cout << angle << "\n\n";
	}

	
	

	Mesh* tank = new Mesh(name);
	tank->InitFromData(vertices, indices);

	return tank;
}
