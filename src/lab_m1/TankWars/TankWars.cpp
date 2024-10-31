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
    this->modelMatrix = glm::mat3(1);
}

TankWars::~TankWars()
{
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



    this->terrain = new Terrain(120.0, 60.0, 20.0, 10.0, 0.005, 0.001, 0.015, 0.035);
    this->terrain->computeHeights(0.0, (float)resolution.x, 10);

    this->terrain->printHeightMap();

    Mesh* terrainMesh = objects::CreateTerrain("terrain", glm::vec3(1, .85f, .34f), terrain->heightMap);
    AddMeshToList(terrainMesh);
}


void TankWars::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void TankWars::Update(float deltaTimeSeconds)
{
    /*this->modelMatrix = glm::mat3(1);
    modelMatrix *= transform::Scale(0.4, 0.75);
    modelMatrix *= transform::Scissor(0, 0.5);
    modelMatrix *= transform::Translate(210, 210);
    RenderMesh2D(meshes["terrSquare"], shaders["VertexColor"], modelMatrix);

    this->modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["terrSquare"], shaders["VertexColor"], modelMatrix);*/

    //modelMatrix = glm::mat3(1);
    ////modelMatrix *= transform::Translate(300, 300);
    ////modelMatrix *= transform::Translate(50, -100);
    //modelMatrix *= transform::Scissor(0, 1.25);

    //modelMatrix *= transform::Translate(50, 100);
    //modelMatrix *= transform::Scale(10, 20);
    //RenderMesh2D(meshes["tudor"], shaders["VertexColor"], modelMatrix);

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