#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace tw
{
	namespace objects
	{
		// Create square with center in the origin with given length an color.
		Mesh* CreateSquare(const std::string& name, float length,
						   glm::vec3 color, bool fill = true);

		Mesh* CreateTerrain(const std::string& name, glm::vec3 color,
							std::vector<std::pair<float, float>> heightMap);
	}
}
