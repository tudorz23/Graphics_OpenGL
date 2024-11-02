#include "lab_m1/TankWars/Missile.h"

#include "utils/glm_utils.h"

using namespace std;
using namespace tw;


// Constructor.
Missile::Missile(float startingX, float startingY, float angle,
				 float power, float gravity, float limit)
{
	this->posX = startingX;
	this->posY = startingY;

	this->speedX = power + 100.f;
	this->speedY = power;

	this->gravity = gravity;

	this->limit = limit;
	this->active = true;
}


void Missile::UpdatePosition(float deltaTime)
{
	this->posX += this->speedX * deltaTime;
	this->posY += this->speedY * deltaTime;

	this->speedY -= this->gravity * deltaTime;

	if (this->posX <= 0 || this->posX >= limit || this->posY <= 0) {
		this->active = false;
	}
}
