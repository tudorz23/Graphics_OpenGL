#include "lab_m1/TankWars/Tank.h"

#include "lab_m1/TankWars/transform2D.h"

#include "utils/glm_utils.h"
#include <iostream>

using namespace std;
using namespace tw;

// Constructor
Tank::Tank(const std::string& bodyName, const std::string& capName,
		   float posX, float posY, float moveSpeed, float rotateSpeed)
{
	this->bodyName = bodyName;
	this->capName = capName;
	this->posX = posX;
	this->posY = posY;
	this->moveSpeed = moveSpeed;
	this->rotateSpeed = rotateSpeed;

	this->slopeAngle = 0;
	this->bodyMatrix = glm::mat3(1);
	this->capMatrix = glm::mat3(1);
}


void Tank::translate(float translateX, float translateY)
{
	this->bodyMatrix *= transform::Translate(translateX, translateY);
	this->capMatrix *= transform::Translate(translateX, translateY);
}


void Tank::rotate(float angle)
{
	this->bodyMatrix *= transform::Rotate(angle);
	this->capMatrix *= transform::Rotate(angle);
}


void Tank::resetMatrixes()
{
	this->bodyMatrix = glm::mat3(1);
	this->capMatrix = glm::mat3(1);
}


void Tank::orientate(const std::vector<std::pair<float, float>> &heightMap)
{
	for (int i = 0; i < heightMap.size() - 1; i++) {
		const auto& pointA = heightMap[i];
		const auto& pointB = heightMap[i + 1];

		if (this->posX >= pointA.first && this->posX <= pointB.first) {
			float t = (this->posX - pointA.first) / (pointB.first - pointA.first);

			this->posY = pointA.second + t * (pointB.second - pointA.second);
			
			float deltaX = pointB.first - pointA.first;
			float deltaY = pointB.second - pointA.second;

			this->slopeAngle = glm::atan2(deltaY, deltaX);
			return;
		}
	}
}
