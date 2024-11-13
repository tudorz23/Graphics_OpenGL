#pragma once

#include <vector>

namespace tw
{
	class Terrain
	{
	 public:
		 Terrain(std::vector<std::pair<float, float>> &sineWaveParams);

		 /**
		  * Calculates the heights for each point x from start to end,
		  * with an interval between them of intervalSpace.
		  */
		 void computeHeights(float start, float end, float intervalSpace);

		 void printHeightMap();


	 private:
		float generatingFunction(float x);

		std::vector<std::pair<float, float>> sineWaveParams;

	 public:
		std::vector<std::pair<float, float>> heightMap;

		// To know if the terrain should be recomputed.
		bool hasChanged;
	};
}
