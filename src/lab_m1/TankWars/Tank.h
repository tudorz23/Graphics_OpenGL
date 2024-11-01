#pragma once

#include <string>
#include <vector>

#include "utils/glm_utils.h"

namespace tw
{
	// A tank is formed from a body and a cap.
	class Tank
	{
	 public:
		Tank(const std::string& bodyName, const std::string& capName,
			  float posX, float posY, float moveSpeed, float rotationSpeed);


		// Utility function over transform::translate().
		void translate(float translateX, float translateY);

		// Utility function over transform::rotate().
		void rotate(float angle);

		// Sets all the modelation matrixes to identity.
		void resetMatrixes();

		// Computes the Y and the next rotation angle.
		void orientate(const std::vector<std::pair<float, float>> &heightMap);

		// Updates the slopeAngle to gradually reach nextAngle value,
		// to prevent a laggy animation.
		void updateOrientation(float deltaTime);
		 

		std::string bodyName;
		std::string capName;

		glm::mat3 bodyMatrix;
		glm::mat3 capMatrix;

		float posX;
		float posY;

		float slopeAngle;
		float nextAngle;

		float moveSpeed;
		float rotationSpeed;
	};
}
