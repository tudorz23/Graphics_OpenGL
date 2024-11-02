#include "lab_m1/TankWars/TankWars.h"

#include "lab_m1/TankWars/constants.h"
#include "lab_m1/TankWars/objects.h"
#include "lab_m1/TankWars/transform2D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace tw;


// Constructor.
TankWars::TankWars()
{
    this->terrain = NULL;
    this->tank1 = NULL;
    this->tank2 = NULL;

    this->modelMatrix = glm::mat3(1);
}


// Destructor.
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
    this->terrain->computeHeights(0.0, (float)resolution.x, 1.5f);


    // Add the terrain mesh.
    Mesh* terrainMesh = objects::CreateTerrain("terrain", TERRAIN_COLOR, terrain->heightMap);
    AddMeshToList(terrainMesh);


    // Add meshes for tank components.
    Mesh* tankBodyMesh1 = objects::CreateTankBody("body1", COLOR_BLACK, COLOR_PURPLE);
    AddMeshToList(tankBodyMesh1);

    Mesh* tankCapMesh1 = objects::CreateSemiCircle("cap1", TANK_CAP_RADIUS, COLOR_BLACK);
    AddMeshToList(tankCapMesh1);

    Mesh* tankPipeMesh1 = objects::CreateRectangle("pipe1", TANK_PIPE_WIDTH, TANK_PIPE_LENGTH, COLOR_BLACK);
    AddMeshToList(tankPipeMesh1);

    this->tank1 = new Tank("body1", "cap1", "pipe1", START_X1, START_Y1, SPEED1, ROTATE_SPEED);


    Mesh* tankBodyMesh2 = objects::CreateTankBody("body2", COLOR_RED, COLOR_YELLOW);
    AddMeshToList(tankBodyMesh2);

    Mesh* tankCapMesh2 = objects::CreateSemiCircle("cap2", TANK_CAP_RADIUS, COLOR_RED);
    AddMeshToList(tankCapMesh2);

    Mesh* tankPipeMesh2 = objects::CreateRectangle("pipe2", TANK_PIPE_WIDTH, TANK_PIPE_LENGTH, COLOR_BLACK);
    AddMeshToList(tankPipeMesh2);

    this->tank2 = new Tank("body2", "cap2", "pipe2", START_X2, START_Y2, SPEED2, ROTATE_SPEED);

    tank1->orientate(terrain->heightMap);
    tank2->orientate(terrain->heightMap);
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);


    tank1->resetMatrixes();
    tank2->resetMatrixes();
}


void TankWars::Update(float deltaTimeSeconds)
{
    // Update the slopes of the tanks gradually at each frame, so there is no lag.
    tank1->updateOrientation(deltaTimeSeconds);
    tank2->updateOrientation(deltaTimeSeconds);

    ApplyTransformationsToTank(tank1);
    ApplyTransformationsToTank(tank2);


    DrawTank(tank1);
    DrawTank(tank2);

    DrawTerrain();
}


void TankWars::FrameEnd()
{
}


void TankWars::ApplyTransformationsToTank(Tank* tank)
{
    // Rotate the tank with slopeAngle.
    // Translate the tank to its current position.
    // Affected: body, cap, pipe.
    tank->translate(tank->posX, tank->posY);
    tank->rotate(tank->slopeAngle);

    // Translate the cap to the top of the tank (this will be done first, so it
    // rotates around the tank's center).
    // Affected: cap.
    tank->capMatrix *= transform::Translate(0, TANK_TOTAL_HEIGHT);


    // Rotate the pipe around its own center, than translate it
    // to the cap's center position.
    // Affected: pipe.
    tank->pipeMatrix *= transform::Translate(tank->pipeX, tank->pipeY);
    tank->pipeMatrix *= transform::Rotate(tank->pipeAngle);
}


void TankWars::DrawTerrain()
{
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], modelMatrix);
}


void TankWars::DrawTank(Tank* tank)
{
    // Render body.
    RenderMesh2D(meshes[tank->bodyName], shaders["VertexColor"], tank->bodyMatrix);
    
    // Render cap.
    RenderMesh2D(meshes[tank->capName], shaders["VertexColor"], tank->capMatrix);

    // Render pipe.
    RenderMesh2D(meshes[tank->pipeName], shaders["VertexColor"], tank->pipeMatrix);
}




/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    // Move first tank.
    if (window->KeyHold(GLFW_KEY_A)) {
        if (tank1->posX > LEFT_LIMIT) {
            tank1->posX -= tank1->moveSpeed * deltaTime;
            tank1->orientate(terrain->heightMap);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        if (tank1->posX < RIGHT_LIMIT) {
            tank1->posX += tank1->moveSpeed * deltaTime;
            tank1->orientate(terrain->heightMap);
        }
    }

    // Rotate first tank's pipe.
    if (window->KeyHold(GLFW_KEY_W)) {
        tank1->pipeAngle -= tank1->pipeRotationSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        tank1->pipeAngle += tank1->pipeRotationSpeed * deltaTime;
    }

    // Move second tank.
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        if (tank2->posX > LEFT_LIMIT) {
            tank2->posX -= tank2->moveSpeed * deltaTime;
            tank2->orientate(terrain->heightMap);
        }
    }

    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        if (tank2->posX < RIGHT_LIMIT) {
            tank2->posX += tank2->moveSpeed * deltaTime;
            tank2->orientate(terrain->heightMap);
        }
    }

    // Rotate second tank's pipe.
    if (window->KeyHold(GLFW_KEY_UP)) {
        tank2->pipeAngle -= tank2->pipeRotationSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_DOWN)) {
        tank2->pipeAngle += tank2->pipeRotationSpeed * deltaTime;
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
