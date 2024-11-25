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
	}
}
