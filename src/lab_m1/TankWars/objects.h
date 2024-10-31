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

		// Create the terrain based on heightMap, using TRIANGLE_STRIP.
		Mesh* CreateTerrain(const std::string& name, glm::vec3 color,
							std::vector<std::pair<float, float>> heightMap);


		// Create a trapezoid with the center being the center of the rectangle,
		// and positioned in (0, 0).
		Mesh* CreateTrapezoid(const std::string& name, float length, float height,
							  glm::vec3 color);


		// Create a semi circle with the center in (0, 0).
		Mesh* CreateSemiCircle(const std::string& name, float radius, glm::vec3 color);


		// Create a tank with the middle of the bottom trpaezoid as the origin.
		Mesh* CreateTank(const std::string& name, glm::vec3 color1, glm::vec3 color2);
	}
}
