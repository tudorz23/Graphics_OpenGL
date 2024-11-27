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

            vertices.emplace_back(glm::vec3(coordX, 0.0f, coordZ), baseColor);
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


Mesh* objects3d::CreateCylinder(const std::string& name, float radius, float height,
								int num_slices, glm::vec3 color)
{
    float halfHeight = height / 2.0f;

    glm::vec3 bottomCenter = glm::vec3(0, -halfHeight, 0);
    glm::vec3 topCenter = glm::vec3(0, halfHeight, 0);

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.emplace_back(bottomCenter, color); // index 0
    vertices.emplace_back(topCenter, color);    // index 1

    float angle_step = 2.0f * (float) M_PI / (float) num_slices;

    // Add vertices for bottom and top circle (num_slices vertices).
    for (int i = 0; i < num_slices; i++)
    {
        float curr_angle = angle_step * i;

        float x = -radius * glm::sin(curr_angle);
        float z = radius * glm::cos(curr_angle);

        // Vertex from bottom circle.
        vertices.emplace_back(glm::vec3(x, -halfHeight, z), color);

        // Vertex from top circle.
        vertices.emplace_back(glm::vec3(x, halfHeight, z), color);
    }

    // Add indices for the circles.
    for (int i = 0; i < num_slices; i++)
    {
        // Triangle for bottom circle.
        indices.push_back(0);
        indices.push_back(2 + i * 2);
        indices.push_back(2 + ((i + 1) % num_slices) * 2);

        // Triangle for top circle.
        indices.push_back(1);
        indices.push_back(3 + i * 2);
        indices.push_back(3 + ((i + 1) % num_slices) * 2);
    }

    //// Add indices for the top circle.
    //for (int i = 0; i < num_slices; i++)
    //{
    //    indices.push_back(1);
    //    indices.push_back(3 + i * 2);
    //    indices.push_back(3 + ((i + 1) % num_slices) * 2);
    //}

    // Add indices for the sides.
    for (int i = 0; i < num_slices; i++)
    {
        int bottom1 = 2 + i * 2;
        int bottom2 = 2 + ((i + 1) % num_slices) * 2;

        int top1 = 3 + i * 2;
        int top2 = 3 + ((i + 1) % num_slices) * 2;

        indices.push_back(bottom1);
        indices.push_back(bottom2);
        indices.push_back(top1);

        indices.push_back(bottom2);
        indices.push_back(top2);
        indices.push_back(top1);
    }

    Mesh* cylinder = new Mesh(name);
    cylinder->InitFromData(vertices, indices);
    return cylinder;
}
