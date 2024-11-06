#pragma once

#include <vector>

namespace tw
{
	class Bird
	{
	 public:
		 // Constructor.
		Bird(float startingX, float moveSpeed, float limit,
			 std::vector<std::pair<float, float>>& sineWaveParams);

		void UpdatePosition(float deltaTime);

		float WaveFunction(float x);

		float posX;
		float posY;
		float moveSpeed;
		float limit;


	 private:
		 std::vector<std::pair<float, float>> sineWaveParams;
	};
}
