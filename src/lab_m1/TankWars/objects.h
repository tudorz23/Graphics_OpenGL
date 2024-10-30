#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace tw
{
	namespace objects
	{
		// Create square with center in the origin with given length an color.
		Mesh* CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill = true);
	}
}