#include "lab_m1/DroneGame/Drone.h"

#include "game_constants.h"
#include "lab_m1/DroneGame/transforms3D.h"

#include "utils/glm_utils.h"


using namespace std;
using namespace drone_game;



/* ---------- Drone methods ---------- */

// Constructor.
Drone::Drone(glm::vec3 startingPos, float startingAngle, float inclination1, float inclination2)
{
	this->position = startingPos;
	this->rotationAngle = startingAngle;

	this->bar1 = new DroneBar(inclination1);
	this->bar2 = new DroneBar(inclination2);
}


void Drone::translate(const glm::vec3 delta)
{
	this->bar1->translate(delta);
	this->bar2->translate(delta);
}


void Drone::rotateOY(const float delta)
{
	this->bar1->rotateOY(delta);
	this->bar2->rotateOY(delta);
}


void Drone::prepareForRender()
{
	// Translate the drone to its current position.
	translate(this->position);

	// Rotate the drone with its current angle.
	rotateOY(this->rotationAngle);

	// Incline the bars so they are x-shaped.
	this->bar1->rotateOY(this->bar1->inclination);
	this->bar2->rotateOY(this->bar2->inclination);

	// Place the attachments (the cubes and the propellers) on each
	// bar's ends, so they automatically rotate with the bar.
	this->bar1->alignAttachments();
	this->bar2->alignAttachments();
}


void Drone::updatePropellerAngle(float deltaTime)
{
	this->bar1->updatePropellerAngle(deltaTime);
	this->bar2->updatePropellerAngle(deltaTime);
}



void Drone::resetModelMatrix()
{
	this->bar1->resetModelMatrix();
	this->bar2->resetModelMatrix();
}



/* ---------- DroneBar methods ---------- */

// Constructor.
DroneBar::DroneBar(float inclination)
{
	this->inclination = inclination;


	this->attachment1 = new DroneAttachment(-DRONE_BODY_LEN / 2.0f + DRONE_CUBE_DIM / 2.0f,
											DRONE_CUBE_DIM,
									 DRONE_CUBE_DIM / 2.0f + DRONE_PROP_HEIGHT / 2.0f);
	this->attachment2 = new DroneAttachment(DRONE_BODY_LEN / 2.0f - DRONE_CUBE_DIM / 2.0f,
											DRONE_CUBE_DIM,
									 DRONE_CUBE_DIM / 2.0f + DRONE_PROP_HEIGHT / 2.0f);

	this->modelMatrix = glm::mat4(1);
}


void DroneBar::translate(glm::vec3 delta)
{
	// Translate the body.
	this->modelMatrix *= transf::Translate(delta.x, delta.y, delta.z);

	// Translate the attachments.
	this->attachment1->translate(delta);
	this->attachment2->translate(delta);
}


void DroneBar::rotateOY(float delta)
{
	// Rotate the body.
	this->modelMatrix *= transf::RotateOY(delta);

	// Rotate the attachments.
	this->attachment1->rotateOY(delta);
	this->attachment2->rotateOY(delta);
}


void DroneBar::alignAttachments()
{
	this->attachment1->alignWithBar();
	this->attachment2->alignWithBar();
}


void DroneBar::updatePropellerAngle(float deltaTime)
{
	this->attachment1->updatePropellerAngle(deltaTime);
	this->attachment2->updatePropellerAngle(deltaTime);
}



void DroneBar::resetModelMatrix()
{
	this->modelMatrix = glm::mat4(1);
	this->attachment1->resetModelMatrix();
	this->attachment2->resetModelMatrix();
}



/* ---------- DroneAttachment methods ---------- */

// Constructor.
DroneAttachment::DroneAttachment(float displacementOX, float displacementOY, float propellerDisplacementOY)
{
	this->displacementOX = displacementOX;
	this->displacementOY = displacementOY;
	this->propellerDisplacementOY = propellerDisplacementOY;

	this->propellerAngle = 0.0f;

	this->cubeMatrix = glm::mat4(1);
	this->propellerMatrix = glm::mat4(1);
}


void DroneAttachment::translate(glm::vec3 delta)
{
	this->cubeMatrix *= transf::Translate(delta.x, delta.y, delta.z);
	this->propellerMatrix *= transf::Translate(delta.x, delta.y, delta.z);
}


void DroneAttachment::rotateOY(float delta)
{
	this->cubeMatrix *= transf::RotateOY(delta);
	this->propellerMatrix *= transf::RotateOY(delta);
}


void DroneAttachment::alignWithBar()
{
	// Translate the cube and the propeller to the
	// end of the bar (the bar is still aligned with OX).
	this->translate(glm::vec3(displacementOX, displacementOY, 0.0f));

	// Translate the propeller to the top of the cube.
	this->propellerMatrix *= transf::Translate(0.0f, propellerDisplacementOY, 0.0f);

	// Rotate the propeller around its own OY axis.
	this->propellerMatrix *= transf::RotateOY(this->propellerAngle);
}


void DroneAttachment::updatePropellerAngle(float deltaTime)
{
	this->propellerAngle += PROPELLER_ROTATION_SPEED * deltaTime;

	if (this->propellerAngle > M_PI * 2.0f)
	{
		this->propellerAngle -= M_PI * 2.0f;
	}
}


void DroneAttachment::resetModelMatrix()
{
	this->cubeMatrix = glm::mat4(1);
	this->propellerMatrix = glm::mat4(1);
}
