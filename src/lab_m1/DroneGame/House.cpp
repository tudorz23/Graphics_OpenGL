#include "lab_m1/DroneGame/House.h"

#include "lab_m1/DroneGame/transforms3D.h"
#include "game_constants.h"

using namespace std;
using namespace drone_game;


House::House(glm::vec3 position, float scaleFactor)
{
	this->position = position;
	this->scaleFactor = scaleFactor;

	this->modelMatrix = glm::mat4(1);


	this->minX = position.x - HOUSE_LEN / 2 * scaleFactor;
	this->maxX = position.x + HOUSE_LEN / 2 * scaleFactor;

	this->minY = 0.0f;
	this->maxY = HOUSE_HEIGHT * scaleFactor;

	this->minZ = position.z - HOUSE_WIDTH / 2 * scaleFactor;
	this->maxZ = position.z + HOUSE_WIDTH / 2 * scaleFactor;
}


void House::prepareForRender()
{
	// Translate to the final position.
	this->modelMatrix *= transf::Translate(position.x, position.y, position.z);

	// Translate the house to surface level.
	this->modelMatrix *= transf::Translate(0, HOUSE_HEIGHT * scaleFactor / 2, 0);

	// Scale.
	this->modelMatrix *= transf::Scale(scaleFactor, scaleFactor, scaleFactor);
}


void House::resetModelMatrix()
{
	this->modelMatrix = glm::mat4(1);
}
