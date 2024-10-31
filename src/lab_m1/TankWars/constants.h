#pragma once

namespace tw
{
	// Background color params.
	constexpr float BACKGROUND_RED = 0.67f;
	constexpr float BACKGROUND_GREEN = 0.84f;
	constexpr float BACKGROUND_BLUE = 0.9f;
	constexpr float BACKGROUND_ALPHA = 1;

	constexpr glm::vec3 TERRAIN_COLOR = glm::vec3(1, .85f, .34f);
	constexpr float TERRAIN_BASE_HEIGHT = 250.0f;

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

	// Tank measurements.
	constexpr float TANK_UPPER_LENGTH = 80.0f;
	constexpr float TANK_UPPER_HEIGHT = 20.0f;
	constexpr float TANK_UPPER_TR_LEN = 12.0f;

	constexpr float TANK_LOWER_LENGTH = 60.0f;
	constexpr float TANK_LOWER_HEIGHT = 12.0f;
	constexpr float TANK_LOWER_TR_LEN = 8.0f;

	constexpr float TANK_TOTAL_HEIGHT = TANK_UPPER_HEIGHT + TANK_LOWER_HEIGHT;
	constexpr float TANK_CAP_RADIUS = 18.0f;


	// Tank specifics.
	constexpr float START_X1 = 400.f;
	constexpr float START_Y1 = 400.f;
	constexpr float SPEED1 = 100.f;

	constexpr float START_X2 = 800.f;
	constexpr float START_Y2 = 400.f;
	constexpr float SPEED2 = 100.f;

	constexpr float ROTATE_SPEED = -5.f;
}
