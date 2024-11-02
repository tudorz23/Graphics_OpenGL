#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace tw
{
	namespace objects
	{
		// Create the terrain based on heightMap, using TRIANGLE_STRIP.
		Mesh* CreateTerrain(const std::string& name, glm::vec3 color,
							std::vector<std::pair<float, float>> heightMap);


		// Create a semi circle, with the center in (0, 0).
		Mesh* CreateSemiCircle(const std::string& name, float radius, glm::vec3 color);


		// Create the tank body, with the middle of the bottom trapezoid in (0, 0).
		Mesh* CreateTankBody(const std::string& name, glm::vec3 low_color, glm::vec3 up_color);


		// Create a rectangle, with the center of the base line (the width) in (0, 0).
		Mesh* CreatePipe(const std::string& name, float width, float length, glm::vec3 color);


		// Create a life bar, with the center of the base line in (0, 0).
		Mesh* CreateLifeBar(const std::string& name, float length, float width,
							float thick, glm::vec3 color);


		// Create a circle, with the center in (0, 0).
		Mesh* CreateCircle(const std::string& name, float radius, glm::vec3 color);


		/* Following seemed necessary but ended up not used. */

		// Create square with center in the origin with given length an color.
		Mesh* CreateSquare(const std::string& name, float length,
							glm::vec3 color, bool fill = true);

		// Create a trapezoid with the center being the center of the rectangle,
		// and positioned in (0, 0).
		Mesh* CreateTrapezoid(const std::string& name, float length, float height,
			glm::vec3 color);
	}
}
