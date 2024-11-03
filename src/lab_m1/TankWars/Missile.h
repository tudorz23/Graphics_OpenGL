#pragma once


namespace tw
{
	class Missile
	{
	 public:
		// Constructor.
		Missile(float startingX, float startingY, float angle,
				float power, float gravity, float limit, float radius,
				short launcher);


		// Updates the position and the speed of the missile after delta time seconds.
		void UpdatePosition(float deltaTime);


		// Position and movement attributes.
		float posX;
		float posY;

		float speedX;
		float speedY;

		float gravity;

		//  Indicates if the missile should be displayed.
		bool active;

		// The right-most point the missile can be at.
		float limit;

		// For colission detection.
		float radius;

		// 1 or 2, to know which tank launced it.
		short launcher;
	};
}
