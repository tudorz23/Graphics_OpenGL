#pragma once

#include "utils/glm_utils.h"

namespace drone_game
{
	class House
	{
	 public:
		glm::mat4 modelMatrix;
		glm::vec3 position;
		float scaleFactor;

		float minX;
		float maxX;
		float minY;
		float maxY;
		float minZ;
		float maxZ;


		// Constructor
		House(glm::vec3 position, float scaleFactor);

		void prepareForRender();

		void resetModelMatrix();
	};
}
