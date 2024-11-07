#include "lab_m1/TankWars/Tank.h"

#include "lab_m1/TankWars/transform2D.h"
#include "lab_m1/TankWars/constants.h"

#include "utils/glm_utils.h"

using namespace std;
using namespace tw;


// Constructor.
Tank::Tank(const std::string& bodyName, const std::string& headName,
		   const std::string& pipeName,
		   float posX, float posY, float moveSpeed, float rotationSpeed)
{
	// Names.
	this->bodyName = bodyName;
	this->headName = headName;
	this->pipeName = pipeName;

	// Body and head params.
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

	// Initialize model matrixes.
	this->resetMatrixes();

	// Initial number of lives.
	this->lives = INITIAL_LIVES;

	this->hasMoved = false;
}


void Tank::resetMatrixes()
{
	this->bodyMatrix = glm::mat3(1);
	this->headMatrix = glm::mat3(1);
	this->pipeMatrix = glm::mat3(1);
	this->barMatrix = glm::mat3(1);
}


// Translates the whole tank with the given params.
// Affects: body, head, pipe, bar.
void Tank::translate(float translateX, float translateY)
{
	this->bodyMatrix *= transform::Translate(translateX, translateY);
	this->headMatrix *= transform::Translate(translateX, translateY);
	this->pipeMatrix *= transform::Translate(translateX, translateY);
	this->barMatrix *= transform::Translate(translateX, translateY);
}


// Rotates the tank around its body's center, which is the center of the base line.
// Affects: body, head, pipe.
void Tank::rotate(float angle)
{
	this->bodyMatrix *= transform::Rotate(angle);
	this->headMatrix *= transform::Rotate(angle);

	// When rotating the tank, the pipe doesn't rotate around tank's center,
	// but changes both its x and y coordinates.
	this->pipeX = -TANK_TOTAL_HEIGHT * glm::sin(angle);
	this->pipeY = TANK_TOTAL_HEIGHT * glm::cos(angle);
}


void Tank::orientate(const std::vector<std::pair<float, float>> &heightMap)
{
	// Get the index of the point from the heightMap that has
	// the closest x coord to the left to the x coord of the tank.
	int index = (int)(this->posX / TERRAIN_POINT_INTERV);
	if (index + 1 == heightMap.size()) {
		return;
	}

	const auto& pointA = heightMap[index];
	const auto& pointB = heightMap[index + 1];

	float t = (this->posX - pointA.first) / (pointB.first - pointA.first);

	this->posY = pointA.second + t * (pointB.second - pointA.second) - PLANTED_DEPTH;

	float deltaX = pointB.first - pointA.first;
	float deltaY = pointB.second - pointA.second;

	this->nextAngle = glm::atan2(deltaY, deltaX);

	// Mark that the tank has moved.
	this->hasMoved = true;
}


void Tank::updateOrientation(float deltaTime)
{
	if (glm::abs(this->nextAngle - this->slopeAngle) < TANK_ANGLE_EPSILON) {
		return;
	}

	this->slopeAngle += (this->nextAngle - this->slopeAngle) * this->rotationSpeed * deltaTime;

	this->hasMoved = true;
}


bool Tank::isAlive()
{
	return this->lives > 0;
}


void Tank::decrementLives()
{
	this->lives--;
}


void Tank::incrementLives()
{
	if (this->lives < INITIAL_LIVES) {
		this->lives++;
	}
}


void Tank::computeTrajectory(float limit)
{
	this->trajectory.clear();

	pair<float, float> pipeHeadPos = this->getPipeHeadPos();

	float currX = pipeHeadPos.first;
	float currY = pipeHeadPos.second;

	float speedX = -MISSILE_POW * glm::sin(this->pipeAngle);
	float speedY = MISSILE_POW * glm::cos(this->pipeAngle);

	float deltaTime = TRAJ_DELTA_TIME;
	
	while (currX > 0 && currX < limit && currY > 0) {
		this->trajectory.push_back({ currX, currY });

		currX += speedX * deltaTime;
		currY += speedY * deltaTime;

		speedY -= GRAVITY * deltaTime;
	}
}


std::pair<float, float> Tank::getPipeHeadPos()
{
	float x = this->posX + this->pipeX - glm::sin(this->pipeAngle) * TANK_PIPE_LENGTH;
	float y = this->posY + this->pipeY + glm::cos(this->pipeAngle) * TANK_PIPE_LENGTH;

	return { x, y };
}
