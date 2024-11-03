#pragma once

namespace tw
{
	/* COLORS */
	// Background color params.
	constexpr float BACKGROUND_RED = 0.67f;
	constexpr float BACKGROUND_GREEN = 0.84f;
	constexpr float BACKGROUND_BLUE = 0.9f;
	constexpr float BACKGROUND_ALPHA = 1;

	constexpr glm::vec3 TERRAIN_COLOR = glm::vec3(0.61f, 0.8f, 0.12f);
	constexpr float TERRAIN_BASE_HEIGHT = 250.0f;

	// Tank colors.
	constexpr glm::vec3 COLOR_RED = glm::vec3(1.0f, 0.0f, 0.0f);
	constexpr glm::vec3 COLOR_PURPLE = glm::vec3(0.78f, 0.05f, 0.94f);
	constexpr glm::vec3 COLOR_BROWN = glm::vec3(0.3f, 0.07f, 0.07f);
	constexpr glm::vec3 COLOR_GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
	constexpr glm::vec3 COLOR_GREY = glm::vec3(0.6f, 0.53f, 0.58f);
	constexpr glm::vec3 COLOR_PINK = glm::vec3(0.94f, 0.1f, 0.82f);
	constexpr glm::vec3 COLOR_BLACK = glm::vec3(0.0f, 0.0f, 0.0f);
	constexpr glm::vec3 COLOR_YELLOW = glm::vec3(1.0f, 1.0f, 0.0f);
	constexpr glm::vec3 COLOR_DARK_GREEN = glm::vec3(0.05f, 0.52f, 0.03f);
	

	// Params for the sine wave of the terrain.
	constexpr float A1 = 120.0f;
	constexpr float A2 = 60.0f;
	constexpr float A3 = 25.0f;
	constexpr float A4 = 10.0f;
	constexpr float OMEGA1 = 0.005f;
	constexpr float OMEGA2 = 0.001f;
	constexpr float OMEGA3 = 0.015f;
	constexpr float OMEGA4 = 0.0355f;


	// For semicircle drawing.
	constexpr int NUM_VERTICES_SEMICIRCLE = 70;

	// For circle drawing.
	constexpr int NUM_VERTICES_CIRCLE = 181;



	// Tank measurements.
	constexpr float TANK_UPPER_LENGTH = 80.0f;
	constexpr float TANK_UPPER_HEIGHT = 20.0f;
	constexpr float TANK_UPPER_TR_LEN = 12.0f;

	constexpr float TANK_LOWER_LENGTH = 60.0f;
	constexpr float TANK_LOWER_HEIGHT = 12.0f;
	constexpr float TANK_LOWER_TR_LEN = 8.0f;

	constexpr float TANK_TOTAL_HEIGHT = TANK_UPPER_HEIGHT + TANK_LOWER_HEIGHT;
	constexpr float TANK_HEAD_RADIUS = 18.0f;

	

	constexpr float TANK_PIPE_LENGTH = 50.0f;
	constexpr float TANK_PIPE_WIDTH = 10.0f;

	constexpr float PLANTED_DEPTH = 3.0f;	// Makes the tank seem planted.

	// For collision detection.
	constexpr float TANK_LOW_TO_CENTER = TANK_LOWER_HEIGHT + TANK_UPPER_HEIGHT / 2.0f;
	constexpr float TANK_RADIUS = TANK_UPPER_LENGTH / 2.0f + TANK_UPPER_TR_LEN;


	// Tank movement parameters.
	constexpr float START_X1 = 400.0f;
	constexpr float START_Y1 = 400.0f;
	constexpr float SPEED1 = 150.f;

	constexpr float START_X2 = 800.0f;
	constexpr float START_Y2 = 400.0f;
	constexpr float SPEED2 = 150.0f;

	constexpr float ROTATE_SPEED = 12.f;
	constexpr float PIPE_ROTATE_SPEED = 2.f;

	constexpr float RIGHT_LIMIT = 1225.0f;
	constexpr float LEFT_LIMIT = 55.0f;


	// Life bar measurements.
	constexpr float BAR_LEN = 80.0f;
	constexpr float BAR_WIDTH = 20.0f;
	constexpr float BAR_THICK = 5.0f;
	constexpr float BAR_DELTA = 100.0f;


	// Missile.
	constexpr float MISSILE_RADIUS = 7.5f;

	// Game logic params.
	constexpr int INITIAL_LIVES = 5;
	constexpr float MISSILE_POW = 400.0f;
	constexpr float GRAVITY = 520.0f;
}
