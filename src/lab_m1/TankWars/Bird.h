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

		float posX;
		float posY;
		float moveSpeed;
		float limit;

		bool active;


	 private:
		 float WaveFunction(float x);

		 std::vector<std::pair<float, float>> sineWaveParams;
	};
}
