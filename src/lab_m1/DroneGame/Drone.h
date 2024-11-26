#pragma once

#include "utils/glm_utils.h"

namespace drone_game
{
	// A DroneAttachment is formed from a cube and a propeller on top of it.
	class DroneAttachment
	{
	 public:

		// The attachment will first be translated on top of the end of its bar
		// while the bar is still aligned with the OX axis.
		float displacementOX;
		float displacementOY;

		float propellerDisplacementOY;


		// The propeller rotates independently around its own OY axis.
		float propellerAngle;


		glm::mat4 cubeMatrix;
		glm::mat4 propellerMatrix;


		DroneAttachment(float displacementOX, float displacementOY, float propellerDisplacementOY);

		void translate(glm::vec3 delta);

		void rotateOY(float delta);


		// Moves the cube and the propeller to the end of the bar they sit on.
		void alignWithBar();


		void updatePropellerAngle(float deltaTime);


		void resetModelMatrix();
	};



	// A bar is formed from its body and two cubes,
	// one on each end.
	class DroneBar
	{
	 public:
		DroneAttachment* attachment1;	// left end attachment
		DroneAttachment* attachment2;	// right end attachment

		glm::mat4 modelMatrix;


		float inclination;



		DroneBar(float inclination);


		void translate(glm::vec3 delta);


		void rotateOY(float delta);


		// Calls the method from DroneAttachment.
		void alignAttachments();


		void updatePropellerAngle(float deltaTime);


		void resetModelMatrix();
	};



	// A drone is formed from two bars placed in x-shape.
	class Drone
	{
	 public:
		DroneBar *bar1; // inclined counterclockwise
		DroneBar *bar2; // inclined clockwise

		glm::vec3 position;

		float rotationAngle;


		Drone(glm::vec3 startingPos, float startingAngle, float inclination1, float inclination2);


		void translate(glm::vec3 delta);

		void rotateOY(float delta);


		// Executes the matrix multiplications necessary to place all the objects
		// that compose the drone to their current position.
		void prepareForRender();


		void updatePropellerAngle(float deltaTime);


		void resetModelMatrix();
	};
}
