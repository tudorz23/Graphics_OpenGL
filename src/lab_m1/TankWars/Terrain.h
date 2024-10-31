#pragma once

#include <vector>

namespace tw
{
	class Terrain
	{
	 public:
		 Terrain(float a1, float a2, float a3, float a4,
				 float omega1, float omega2, float omega3, float omega4);

		 /**
		  * Calculates the heights for each point x from start to end,
		  * with an interval between them of intervalSpace.
		  */
		 void computeHeights(float start, float end, float intervalSpace);

		 void printHeightMap();


	 private:
		float generatingFunction(float x);


		float a1, a2, a3, a4;
		float omega1, omega2, omega3, omega4;

	public:
		
		 std::vector<std::pair<float, float>> heightMap;

	};
}