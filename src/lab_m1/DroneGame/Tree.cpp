#include "lab_m1/DroneGame/Tree.h"

#include "lab_m1/DroneGame/transforms3D.h"
#include "game_constants.h"

using namespace std;
using namespace drone_game;


Tree::Tree(glm::vec3 position, float scaleFactor)
{
	this->position = position;
	this->scaleFactor = scaleFactor;

	this->trunkMatrix = glm::mat4(1);
	this->topConeMatrix = glm::mat4(1);
	this->bottomConeMatrix = glm::mat4(1);

	this->trunkElevation = CYLINDER_HEIGHT * scaleFactor / 2;
	this->trunkCenter = this->position + glm::vec3(0, trunkElevation, 0);
	this->trunkRadius = CYLINDER_RADIUS * scaleFactor;

}


void Tree::translate(glm::vec3 delta)
{
	this->trunkMatrix *= transf::Translate(delta.x, delta.y, delta.z);
	this->topConeMatrix *= transf::Translate(delta.x, delta.y, delta.z);
	this->bottomConeMatrix *= transf::Translate(delta.x, delta.y, delta.z);
}


void Tree::prepareForRender()
{
	// Translate to the final position.
	translate(position);

	// Translate the trunk to surface level.
	this->trunkMatrix *= transf::Translate(0, this->trunkElevation, 0);

	// Translate the bottom cone on top of the trunk.
	this->bottomConeMatrix *= transf::Translate(0, CONE_HEIGHT * scaleFactor / 4 + CYLINDER_HEIGHT * scaleFactor, 0);

	// Translate the top cone.
	this->topConeMatrix *= transf::Translate(0, CONE_HEIGHT * scaleFactor / 2 + CYLINDER_HEIGHT * scaleFactor, 0);

	// Scale every component.
	this->trunkMatrix *= transf::Scale(scaleFactor, scaleFactor, scaleFactor);
	this->topConeMatrix *= transf::Scale(scaleFactor, scaleFactor, scaleFactor);
	this->bottomConeMatrix *= transf::Scale(scaleFactor, scaleFactor, scaleFactor);
}



void Tree::resetModelMatrix()
{
	this->trunkMatrix = glm::mat4(1);
	this->topConeMatrix = glm::mat4(1);
	this->bottomConeMatrix = glm::mat4(1);
}

