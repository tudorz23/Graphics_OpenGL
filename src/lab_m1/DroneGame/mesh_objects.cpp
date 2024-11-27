#include "lab_m1/DroneGame/mesh_objects.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace drone_game;


Mesh* objects3d::CreateParallelepiped(const std::string& name, float length,
										float width, float height, glm::vec3 color)
{
    float halfLen = length / 2.0f;
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-halfLen, -halfHeight, halfWidth), color),
        VertexFormat(glm::vec3(-halfLen, -halfHeight, -halfWidth), color),
        VertexFormat(glm::vec3(-halfLen, halfHeight, -halfWidth), color),
        VertexFormat(glm::vec3(-halfLen, halfHeight, halfWidth), color),
        VertexFormat(glm::vec3(halfLen, -halfHeight, halfWidth), color),
        VertexFormat(glm::vec3(halfLen, -halfHeight, -halfWidth), color),
        VertexFormat(glm::vec3(halfLen, halfHeight, -halfWidth), color),
        VertexFormat(glm::vec3(halfLen, halfHeight, halfWidth), color)
	};

    std::vector<unsigned int> indices =
    {
        1, 0, 2,
        0, 3, 2,
        4, 5, 6,
        4, 6, 7,

        0, 4, 3,
        3, 4, 7,
        1, 2, 5,
        2, 6, 5,

        3, 6, 2,
        3, 7, 6,
        0, 1, 5,
        0, 5, 4
    };

    Mesh* parallelepiped = new Mesh(name);
    parallelepiped->InitFromData(vertices, indices);
    return parallelepiped;
}


// Build a grid with m rows and n columns, with all the cells being identical rectangles.
//
// If there are m rows, there are m + 1 lines creating them.
// Similarly, there are n + 1 lines creating the columns.
// There should be a vertex at each intersection between these lines, so (m + 1) * (n + 1).
//
// Use even values for m and n, so the center of the grid will be at lines (m/2, n/2).
//
// On rows, the Z coordinate will vary.
// On columns, the X coordinate will vary.
Mesh* objects3d::CreateTerrain(const std::string& name, int m, int n,
							   float cellLen, float cellWidth, glm::vec3 baseColor)
{
    int centerRowLine = m / 2;
    int centerColLine = n / 2;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Add vertices.
	for (int i = 0; i <= m; i++)
	{
        float coordZ = (i - centerRowLine) * cellWidth;

		for (int j = 0; j <= n; j++)
		{
            float coordX = (j - centerColLine) * cellLen;

            vertices.push_back(VertexFormat(glm::vec3(coordX, 0.0f, coordZ), baseColor));
		}
	}

    int vertsPerRow = n + 1;

    // Add indices, building triangles.
    // For each cell, there will be two triangles.
    for (int i = 0; i < m; i++)
    {
	    for (int j = 0; j < n; j++)
	    {
            // First triangle.
            indices.push_back(vertsPerRow * i + j);
            indices.push_back(vertsPerRow * (i + 1) + j);
            indices.push_back(vertsPerRow * (i + 1) + j + 1);

            // Second triangle.
            indices.push_back(vertsPerRow * i + j);
            indices.push_back(vertsPerRow * (i + 1) + j + 1);
            indices.push_back(vertsPerRow * i + j + 1);
	    }
    }

    Mesh* terrain = new Mesh(name);
    terrain->InitFromData(vertices, indices);
    return terrain;
}
