#pragma once

#include "utils/glm_utils.h"

namespace drone_game
{
	// A tree is formed by a trunk and two cones.
	class Tree
	{
	 public:
		glm::mat4 trunkMatrix;
		glm::mat4 topConeMatrix;
		glm::mat4 bottomConeMatrix;

		glm::vec3 position;
		float scaleFactor;


		// Constructor
		Tree(glm::vec3 position, float scaleFactor);

		void translate(glm::vec3 delta);

		void prepareForRender();

		void resetModelMatrix();
	};
}
