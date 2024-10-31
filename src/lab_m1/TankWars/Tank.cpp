#include "lab_m1/TankWars/Tank.h"

#include "lab_m1/TankWars/transform2D.h"

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

	this->rotationAngle = 0;
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
