#pragma once

// Colors.
constexpr glm::vec3 COLOR_GREY = glm::vec3(0.6f, 0.53f, 0.58f);
constexpr glm::vec3 COLOR_RED = glm::vec3(0.8f, 0.0f, 0.0f);
constexpr glm::vec3 COLOR_BLACK = glm::vec3(0.0f, 0.0f, 0.0f);
constexpr glm::vec3 COLOR_GREEN = glm::vec3(0.0f, 0.8f, 0.0f);
constexpr glm::vec3 COLOR_PURPLE = glm::vec3(0.78f, 0.05f, 0.94f);
constexpr glm::vec3 COLOR_DARK_BLUE = glm::vec3(0.82, 0.647, 0.376);
constexpr glm::vec3 COLOR_DARK_YELLOW = glm::vec3(0.216, 0.149, 0.612);
constexpr glm::vec3 COLOR_BROWN = glm::vec3(0.478, 0.22, 0.212);
constexpr glm::vec3 COLOR_DARK_GREEN = glm::vec3(0.231, 0.51, 0.137);
constexpr glm::vec3 COLOR_YELLOW = glm::vec3(0.612, 0.6, 0.106);

constexpr glm::vec3 COLOR_DARK_PURPLE = glm::vec3(0.122, 0.043, 0.502);


// Third person camera distances to the drone.
constexpr float FORWARD_DISTANCE = 2.0f;
constexpr float UP_DISTANCE = -0.3f;


// Terrain size constants.
constexpr int TERRAIN_M = 212;
constexpr int TERRAIN_N = 212;
constexpr float TERRAIN_CELL_LEN = 0.6f;
constexpr float TERRAIN_CELL_WIDTH = 0.6f;
constexpr float MAX_TERRAIN_HEIGHT = 0.3f;


// Drone sizes.
constexpr float DRONE_BODY_LEN = 1.0f;
constexpr float DRONE_BODY_WIDTH = 0.1f;
constexpr float DRONE_BODY_HEIGHT = 0.1f;

constexpr float DRONE_CUBE_DIM = 0.1f;
constexpr float DRONE_PROP_LEN = 0.3f;
constexpr float DRONE_PROP_WIDTH = 0.02f;
constexpr float DRONE_PROP_HEIGHT = 0.01f;

constexpr float DRONE_RADIUS = 0.5f;


// Drone movement parameters.
constexpr glm::vec3 DRONE_START_POS = glm::vec3(0, 0, 0);
constexpr float DRONE_BAR_INCL1 = glm::radians(40.0f);
constexpr float DRONE_BAR_INCL2 = glm::radians(-40.0f);

constexpr float DRONE_SPEED = 5.0f;
constexpr float DRONE_ROTATION_SPEED = 2.0f;

constexpr float SENSITIVITY_OX = 0.001f;
constexpr float SENSITIVITY_OY = 0.001f;

constexpr float PROPELLER_ROTATION_SPEED = 25.0f;


// Tree constants.
constexpr float CYLINDER_RADIUS = 0.35f;
constexpr float CYLINDER_HEIGHT = 2.5f;
constexpr int NUM_SLICES = 120;

constexpr float CONE_RADIUS = 1.4f;
constexpr float CONE_HEIGHT = 2.5f;

// tg(alpha) = radius / height.
constexpr float CONE_SLOPE = CONE_RADIUS / CONE_HEIGHT;


// Obstacles constants.
constexpr int NR_TREES = 35;
constexpr float OBSTACLE_SPACE = 3.9f;
constexpr int OBST_MAX_ROW = 31;
constexpr int OBST_MAX_COL = 31;

constexpr int NR_HOUSES = 20;


// House constants.
constexpr float HOUSE_LEN = 3.0f;
constexpr float HOUSE_WIDTH = 3.0f;
constexpr float HOUSE_HEIGHT = 4.0f;

