#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace drone_game
{
	namespace objects3d
	{
		Mesh* CreateParallelepiped(const std::string& name, float length, float width,
									float height, glm::vec3 color);


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
		Mesh* CreateTerrain(const std::string& name, int m, int n,
							float cellLen, float cellWidth, glm::vec3 baseColor);


		// Build a cylinder centered in (0, 0).
		Mesh* CreateCylinder(const std::string& name, float radius, float height,
							 int num_slices, glm::vec3 color);

	}
}
