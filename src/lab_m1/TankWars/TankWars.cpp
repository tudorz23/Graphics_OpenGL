#include "lab_m1/TankWars/TankWars.h"

#include "lab_m1/TankWars/constants.h"
#include "lab_m1/TankWars/objects.h"
#include "lab_m1/TankWars/transform2D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace tw;

TankWars::TankWars()
{
    this->terrain = NULL;
    this->tank1 = NULL;
    this->tank2 = NULL;

    this->modelMatrix = glm::mat3(1);
}

TankWars::~TankWars()
{
    delete terrain;
    delete tank1;
    delete tank2;
}


void TankWars::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Initialize the Terrain class with a sine wave.
    vector<pair<float, float>> sineWaveParams = { {A1, OMEGA1}, {A2, OMEGA2},
                                                  {A3, OMEGA3}, {A4, OMEGA4} };
    this->terrain = new Terrain(sineWaveParams);
    this->terrain->computeHeights(0.0, (float)resolution.x, 10.0f);

    Mesh* terrainMesh = objects::CreateTerrain("terrain", TERRAIN_COLOR, terrain->heightMap);
    AddMeshToList(terrainMesh);


    // Add tank components.

    Mesh* tankBodyMesh1 = objects::CreateTankBody("body1", glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
    AddMeshToList(tankBodyMesh1);

    Mesh* tankCapMesh1 = objects::CreateSemiCircle("cap1", TANK_CAP_RADIUS, glm::vec3(1, 0, 1));
    AddMeshToList(tankCapMesh1);

    Mesh* tankBodyMesh2 = objects::CreateTankBody("body2", glm::vec3(0, 0, 1), glm::vec3(1, 1, 0));
    AddMeshToList(tankBodyMesh2);

    Mesh* tankCapMesh2 = objects::CreateSemiCircle("cap2", TANK_CAP_RADIUS, glm::vec3(0, 1, 1));
    AddMeshToList(tankCapMesh2);

    this->tank1 = new Tank("body1", "cap1", START_X1, START_Y1, SPEED1, ROTATE_SPEED);
    this->tank2 = new Tank("body2", "cap2", START_X2, START_Y2, SPEED2, ROTATE_SPEED);
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    tank1->bodyMatrix = glm::mat3(1);
    tank1->capMatrix = glm::mat3(1);

    tank2->bodyMatrix = glm::mat3(1);
    tank2->capMatrix = glm::mat3(1);
}


void TankWars::Update(float deltaTimeSeconds)
{
    
    tank1->translate(tank1->posX, tank1->posY);
    tank1->rotate(tank1->rotationAngle);

    
    tank2->translate(tank2->posX, tank2->posY);
    tank2->rotate(tank2->rotationAngle);

    DrawTank(tank1);
    DrawTank(tank2);


    DrawTerrain();
}


void TankWars::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        tank1->posY += tank1->moveSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        tank1->posY -= tank1->moveSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        tank1->posX -= tank1->moveSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        tank1->posX += tank1->moveSpeed * deltaTime;
    }


    if (window->KeyHold(GLFW_KEY_Q)) {
        tank1->rotationAngle -= tank1->rotateSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        tank1->rotationAngle += tank1->rotateSpeed * deltaTime;
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void TankWars::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void TankWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void TankWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void TankWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void TankWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void TankWars::OnWindowResize(int width, int height)
{
}


void TankWars::DrawTerrain()
{
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], modelMatrix);
}


void TankWars::DrawTank(Tank *tank)
{
    RenderMesh2D(meshes[tank->bodyName], shaders["VertexColor"], tank->bodyMatrix);

    tank->capMatrix *= transform::Translate(0, TANK_TOTAL_HEIGHT);
    RenderMesh2D(meshes[tank->capName], shaders["VertexColor"], tank->capMatrix);
}