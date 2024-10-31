#include "lab_m1/TankWars/Terrain.h"

#include "utils/glm_utils.h"
#include "lab_m1/TankWars/constants.h"

#include <iostream>

using namespace std;
using namespace tw;


// Constructor
Terrain::Terrain(std::vector<std::pair<float, float>> sineWaveParams)
{
	this->sineWaveParams = sineWaveParams;
}


float Terrain::generatingFunction(float x)
{
	float y = TERRAIN_BASE_HEIGHT;

	for (const auto& param : this->sineWaveParams) {
		y += param.first * glm::sin(param.second * x);
	}

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
