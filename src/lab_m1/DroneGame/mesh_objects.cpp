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
