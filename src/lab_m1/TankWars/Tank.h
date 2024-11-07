#pragma once

#include <string>
#include <vector>

#include "utils/glm_utils.h"

namespace tw
{
	// A tank is formed from a body and a head.
	class Tank
	{
	 public:
		// Constructor.
		Tank(const std::string& bodyName, const std::string& headName,
			 const std::string& pipeName,
			 float posX, float posY, float moveSpeed, float rotationSpeed);


		// Sets all the modelation matrixes to identity matrix.
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


		bool isAlive();

		void decrementLives();

		void incrementLives();


		// Populates the trajectory vector with points that a missile
		// would follow if fired.
		void computeTrajectory(float limit);


		// Returns the position of the head of the pipe.
		std::pair<float, float> getPipeHeadPos();
		 

		// Names.
		std::string bodyName;
		std::string headName;
		std::string pipeName;

		// Body and head params.
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
		glm::mat3 headMatrix;
		glm::mat3 pipeMatrix;
		glm::mat3 barMatrix;

		int lives;

		// For Trajectory hint.
		std::vector<std::pair<float, float>> trajectory;

		// To know when to recompute the trajectory.
		bool hasMoved;
	};
}
