#include "lab_m1/TankWars/Terrain.h"

#include "utils/glm_utils.h"

#include <iostream>

using namespace std;
using namespace tw;


// Constructor
Terrain::Terrain(float a1, float a2, float a3, float a4,
				 float omega1, float omega2, float omega3, float omega4)
{
	this->a1 = a1;
	this->a2 = a2;
	this->a3 = a3;
	this->a4 = a4;
	this->omega1 = omega1;
	this->omega2 = omega2;
	this->omega3 = omega3;
	this->omega4 = omega4;
}


float Terrain::generatingFunction(float x)
{
	float y = 200.f;
	y += a1 * glm::sin(omega1 * x);
	y += a2 * glm::sin(omega2 * x);
	y += a3 * glm::sin(omega3 * x);
	y += a4 * glm::sin(omega4 * x);

	return y;
}


void Terrain::computeHeights(float start, float end, float intervalSpace)
{
	for (float x = start; x <= end; x += intervalSpace) {
		float y = generatingFunction(x);

		this->heightMap.push_back({ x, y });
	}
}


void Terrain::printHeightMap() {
	for (const auto &point : this->heightMap) {
		cout << "x = " << point.first << ", y = " << point.second << "\n";
	}
}
