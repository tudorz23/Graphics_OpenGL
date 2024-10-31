#pragma once

#include <string>

#include "utils/glm_utils.h"

namespace tw
{
	// A tank is formed from a body and a cap.
	class Tank
	{
	 public:
		Tank(const std::string& bodyName, const std::string& capName,
			  float posX, float posY, float moveSpeed, float rotateSpeed);

		void translate(float translateX, float translateY);
		void rotate(float angle);
		 
		std::string bodyName;
		std::string capName;

		glm::mat3 bodyMatrix;
		glm::mat3 capMatrix;

		float posX;
		float posY;

		float rotationAngle;

		float moveSpeed;
		float rotateSpeed;
	};
}
