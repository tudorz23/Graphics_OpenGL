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
		// Constructor.
		Tank(const std::string& bodyName, const std::string& capName,
			 const std::string& pipeName,
			 float posX, float posY, float moveSpeed, float rotationSpeed);


		// Sets all the modelation matrixes to identity.
		void resetMatrixes();


		// Utility function over transform::translate().
		void translate(float translateX, float translateY);


		// Utility function over transform::rotate().
		void rotate(float angle);

		
		// Computes the Y and the next rotation angle.
		void orientate(const std::vector<std::pair<float, float>> &heightMap);


		// Updates the slopeAngle to gradually reach nextAngle value,
		// to prevent a laggy animation.
		void updateOrientation(float deltaTime);
		 

		// Names.
		std::string bodyName;
		std::string capName;
		std::string pipeName;

		// Body and cap params.
		float posX;
		float posY;
		float moveSpeed;

		float slopeAngle;
		float nextAngle;
		float rotationSpeed;

		// Pipe params.
		float pipeX;
		float pipeY;
		float pipeAngle;
		float pipeRotationSpeed;

		// Model matrixes.
		glm::mat3 bodyMatrix;
		glm::mat3 capMatrix;
		glm::mat3 pipeMatrix;
	};
}
