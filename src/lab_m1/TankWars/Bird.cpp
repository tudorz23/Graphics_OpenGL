#include "lab_m1/TankWars/Bird.h"

#include "utils/glm_utils.h"

#include "lab_m1/TankWars/constants.h"


using namespace std;
using namespace tw;


Bird::Bird(float startingX, float moveSpeed, float limit,
		   std::vector<std::pair<float, float>>& sineWaveParams)
{
	this->posX = startingX;
	this->posY = BIRD_BASE_HEIGHT;
	this->moveSpeed = moveSpeed;
	this->limit = limit;
	this->sineWaveParams = sineWaveParams;
}


void Bird::UpdatePosition(float deltaTime)
{
	this->posX += this->moveSpeed * deltaTime;
	if (this->posX >= limit) {
		this->posX = 0.0f;
	}

	this->posY = WaveFunction(this->posX);
}


float Bird::WaveFunction(float x)
{
	float y = BIRD_BASE_HEIGHT;

	for (const auto& param : this->sineWaveParams) {
		y += param.first * glm::sin(param.second * x);
	}

	return y;
}
