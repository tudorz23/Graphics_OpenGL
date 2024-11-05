#include "lab_m1/TankWars/objects.h"

#include "lab_m1/TankWars/constants.h"
#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include <iostream>

using namespace tw;


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


Mesh* objects::CreateSemiCircle(const std::string& name, float radius, glm::vec3 color)
{
	glm::vec3 center = glm::vec3(0, 0, 0);

	std::vector<VertexFormat> vertices = { VertexFormat(center, color) };
	std::vector<unsigned int> indices = { 0 };

	float angle_base = (float) M_PI / (float) (NUM_VERTICES_SEMICIRCLE - 1);

	for (int i = 0; i < NUM_VERTICES_SEMICIRCLE; i++) {
		float angle = angle_base * i;

		float x = radius * glm::cos(angle);
		float y = radius * glm::sin(angle);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		indices.push_back(i + 1);
	}

	Mesh* semicircle = new Mesh(name);
	semicircle->SetDrawMode(GL_TRIANGLE_FAN);
	semicircle->InitFromData(vertices, indices);
	return semicircle;
}


Mesh* objects::CreateTankBody(const std::string& name, glm::vec3 low_color, glm::vec3 up_color)
{
	// Lower trapezoid.
	glm::vec3 lowerBottomLeft = glm::vec3(-TANK_LOWER_LENGTH / 2.f, 0, 0);
	glm::vec3 lowerTopLeft = lowerBottomLeft + glm::vec3(-TANK_LOWER_TR_LEN, TANK_LOWER_HEIGHT, 0);
	glm::vec3 lowerTopRight = lowerBottomLeft + glm::vec3(TANK_LOWER_LENGTH + TANK_LOWER_TR_LEN, TANK_LOWER_HEIGHT, 0);
	glm::vec3 lowerBottomRight = lowerBottomLeft + glm::vec3(TANK_LOWER_LENGTH, 0, 0);

	// Upper trapezoid.
	glm::vec3 upperTopLeft = glm::vec3(-TANK_UPPER_LENGTH / 2.f, TANK_TOTAL_HEIGHT, 0);
	glm::vec3 upperTopRight = upperTopLeft + glm::vec3(TANK_UPPER_LENGTH, 0, 0);
	glm::vec3 upperBottomRight = upperTopRight + glm::vec3(TANK_UPPER_TR_LEN, -TANK_UPPER_HEIGHT, 0);
	glm::vec3 upperBottomLeft = upperTopLeft + glm::vec3(-TANK_UPPER_TR_LEN, -TANK_UPPER_HEIGHT, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(lowerBottomLeft, low_color),
		VertexFormat(lowerTopLeft, low_color),
		VertexFormat(lowerTopRight, low_color),
		VertexFormat(lowerBottomRight, low_color),

		VertexFormat(upperTopLeft, up_color),
		VertexFormat(upperTopRight, up_color),
		VertexFormat(upperBottomRight, up_color),
		VertexFormat(upperBottomLeft, up_color)
	};

	std::vector<unsigned int> indices = { 0, 2, 1, 0, 3, 2,
										  7, 5, 4, 7, 6, 5 };
	
	Mesh* tank = new Mesh(name);
	tank->InitFromData(vertices, indices);
	return tank;
}


Mesh* objects::CreateRectangle(const std::string& name, float width, float length, glm::vec3 color)
{
	float halfWidth = width / 2.0f;
	glm::vec3 bottomLeft = glm::vec3(-halfWidth, 0, 0);
	glm::vec3 bottomRight = glm::vec3(halfWidth, 0, 0);
	glm::vec3 topLeft = glm::vec3(-halfWidth, length, 0);
	glm::vec3 topRight = glm::vec3(halfWidth, length, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(bottomLeft, color),
		VertexFormat(bottomRight, color),
		VertexFormat(topRight, color),
		VertexFormat(topLeft, color)
	};

	std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };

	Mesh* rectangle = new Mesh(name);
	rectangle->InitFromData(vertices, indices);
	return rectangle;
}


/*
*		3	* *				* *
*		2	* *				* *
* 
*			
*		1	* *				* *
*		0	* *				* *
* 
*			0 1				2 3
*/
Mesh* objects::CreateLifeBar(const std::string& name, float length, float width,
							 float thick, glm::vec3 color)
{
	float totalLen = length + 2 * thick;
	float totalWidth = width + 2 * thick;

	glm::vec3 point00 = glm::vec3(-totalLen / 2.0f, 0, 0);
	glm::vec3 point01 = point00 + glm::vec3(thick, 0, 0);
	glm::vec3 point02 = point01 + glm::vec3(length, 0, 0);
	glm::vec3 point03 = point02 + glm::vec3(thick, 0, 0);

	glm::vec3 point10 = point00 + glm::vec3(0, thick, 0);
	glm::vec3 point11 = point10 + glm::vec3(thick, 0, 0);
	glm::vec3 point12 = point11 + glm::vec3(length, 0, 0);
	glm::vec3 point13 = point12 + glm::vec3(thick, 0, 0);

	glm::vec3 point20 = point10 + glm::vec3(0, width, 0);
	glm::vec3 point21 = point20 + glm::vec3(thick, 0, 0);
	glm::vec3 point22 = point21 + glm::vec3(length, 0, 0);
	glm::vec3 point23 = point22 + glm::vec3(thick, 0, 0);

	glm::vec3 point30 = point20 + glm::vec3(0, thick, 0);
	glm::vec3 point31 = point30 + glm::vec3(thick, 0, 0);
	glm::vec3 point32 = point31 + glm::vec3(length, 0, 0);
	glm::vec3 point33 = point32 + glm::vec3(thick, 0, 0);


	std::vector<VertexFormat> vertices =
	{
		VertexFormat(point00, color),
		VertexFormat(point01, color),
		VertexFormat(point02, color),
		VertexFormat(point03, color),

		VertexFormat(point10, color),
		VertexFormat(point11, color),
		VertexFormat(point12, color),
		VertexFormat(point13, color),

		VertexFormat(point20, color),
		VertexFormat(point21, color),
		VertexFormat(point22, color),
		VertexFormat(point23, color),

		VertexFormat(point30, color),
		VertexFormat(point31, color),
		VertexFormat(point32, color),
		VertexFormat(point33, color)
	};

	std::vector<unsigned int> indices =
	{ 0, 1, 4,
	  1, 5, 4,
	  4, 5, 8,
	  5, 9, 8,
	  8, 9, 12,
	  9, 13, 12,
	  9, 10, 13,
	  10, 14, 13,
	  10, 11, 14,
	  11, 15, 14,
	  6, 7, 10,
	  7, 11, 10,
	  2, 3, 6,
	  6, 3, 7,
	  1, 2, 5,
	  2, 6 , 5
	};

	Mesh* lifeBar = new Mesh(name);
	lifeBar->InitFromData(vertices, indices);
	return lifeBar;
}


Mesh* objects::CreateCircle(const std::string& name, float radius, glm::vec3 color)
{
	glm::vec3 center = glm::vec3(0, 0, 0);

	std::vector<VertexFormat> vertices = { VertexFormat(center, color) };
	std::vector<unsigned int> indices = {0};

	float angle_base = (2.0f * (float) M_PI) / (float)(NUM_VERTICES_CIRCLE - 1);

	for (int i = 0; i < NUM_VERTICES_CIRCLE; i++) {
		float angle = angle_base * i;

		float x = radius * glm::cos(angle);
		float y = radius * glm::sin(angle);

		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		indices.push_back(i + 1);
	}

	Mesh* circle = new Mesh(name);
	circle->SetDrawMode(GL_TRIANGLE_FAN);
	circle->InitFromData(vertices, indices);
	return circle;
}



/* Following seemed useful but ended up not used. */
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
