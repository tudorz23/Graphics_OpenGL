#include "lab_m1/TankWars/Missile.h"

#include "utils/glm_utils.h"

using namespace std;
using namespace tw;


// Constructor.
Missile::Missile(float startingX, float startingY, float angle,
				 float power, float gravity, float limit, float radius,
				 short launcher)
{
	this->posX = startingX;
	this->posY = startingY;

	// Power is distributed as speed to X and Y components, depending on the angle.
	this->speedX = -power * glm::sin(angle);
	this->speedY = power * glm::cos(angle);

	this->gravity = gravity;

	this->limit = limit;
	this->active = true;

	this->radius = radius;

	this->launcher = launcher;
}


void Missile::UpdatePosition(float deltaTime)
{
	this->posX += this->speedX * deltaTime;
	this->posY += this->speedY * deltaTime;

	this->speedY -= this->gravity * deltaTime;

	if (this->posX <= 0 || this->posX >= limit || this->posY <= 0) {
		this->MarkInactive();
	}
}


void Missile::MarkInactive()
{
	this->active = false;
}
