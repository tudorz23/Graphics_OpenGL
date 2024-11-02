#include "lab_m1/TankWars/Tank.h"

#include "lab_m1/TankWars/transform2D.h"
#include "lab_m1/TankWars/constants.h"

#include "utils/glm_utils.h"

#include <iostream>


using namespace std;
using namespace tw;

// Constructor
Tank::Tank(const std::string& bodyName, const std::string& capName,
		   const std::string& pipeName,
		   float posX, float posY, float moveSpeed, float rotationSpeed)
{
	// Names.
	this->bodyName = bodyName;
	this->capName = capName;
	this->pipeName = pipeName;

	// Body and cap params.
	this->posX = posX;
	this->posY = posY;
	this->moveSpeed = moveSpeed;
	
	this->slopeAngle = 0;
	this->nextAngle = 0;
	this->rotationSpeed = rotationSpeed;

	// Pipe params.
	this->pipeX = 0.0f;
	this->pipeY = 0.0f;
	this->pipeAngle = 0.0f;
	this->pipeRotationSpeed = PIPE_ROTATE_SPEED;

	this->resetMatrixes();
}


void Tank::translate(float translateX, float translateY)
{
	this->bodyMatrix *= transform::Translate(translateX, translateY);
	this->capMatrix *= transform::Translate(translateX, translateY);
	this->pipeMatrix *= transform::Translate(translateX, translateY);
}


void Tank::rotate(float angle)
{
	this->bodyMatrix *= transform::Rotate(angle);
	this->capMatrix *= transform::Rotate(angle);

	// When rotating the tank, the pipe doesn't rotate around tank's center,
	// but changes both its x and y coordinates.
	this->pipeX = -TANK_TOTAL_HEIGHT * glm::sin(angle);
	this->pipeY = TANK_TOTAL_HEIGHT * glm::cos(angle);
}


void Tank::resetMatrixes()
{
	this->bodyMatrix = glm::mat3(1);
	this->capMatrix = glm::mat3(1);
	this->pipeMatrix = glm::mat3(1);
}


void Tank::orientate(const std::vector<std::pair<float, float>> &heightMap)
{
	std::cout << "Lewis Hamilton\n";

	for (int i = 0; i < heightMap.size() - 1; i++) {
		const auto& pointA = heightMap[i];
		const auto& pointB = heightMap[i + 1];

		if (this->posX >= pointA.first && this->posX <= pointB.first) {
			float t = (this->posX - pointA.first) / (pointB.first - pointA.first);

			this->posY = pointA.second + t * (pointB.second - pointA.second) - PLANTED_DEPTH;
			
			float deltaX = pointB.first - pointA.first;
			float deltaY = pointB.second - pointA.second;

			this->nextAngle = glm::atan2(deltaY, deltaX);
			return;
		}
	}
}


void Tank::updateOrientation(float deltaTime)
{
	this->slopeAngle += (this->nextAngle - this->slopeAngle) * this->rotationSpeed * deltaTime;
}
