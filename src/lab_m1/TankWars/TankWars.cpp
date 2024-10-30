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


    // Draw a square.
    /*Mesh* square = objects::CreateSquare("tudor", 400, glm::vec3(1, .85f, .34f));
    AddMeshToList(square);*/

    Mesh* terrSquare = objects::CreateSquare("terrSquare", 50, glm::vec3(1, .85f, .34f));
    AddMeshToList(terrSquare);


    this->terrain = new Terrain(1.0, 2.0, 0.5, 1.0, 0.5, 3.0);
    this->terrain->computeHeights(0.0, (float)resolution.x, 7);

    this->terrain->printHeightMap();
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


void TankWars::DrawTerrain() {
    for (int i = 0; i < terrain->heightMap.size() - 1; i++) {
        auto pointA = terrain->heightMap[i];
        auto pointB = terrain->heightMap[i + 1];

        float scaleX = pointB.first - pointA.first;
        float scaleY = max(pointB.second, pointA.second);

        //cout << "Scale x = " << scaleX << ", Scale y = " << scaleY << "\n";

        float scissorY = (pointB.second - pointA.second) / (pointB.first - pointA.first);

        modelMatrix = glm::mat3(1);

        modelMatrix *= transform::Translate(pointA.first, pointA.second);
        modelMatrix *= transform::Scissor(0, scissorY);
        modelMatrix *= transform::Scale(scaleX, scaleY);

        RenderMesh2D(meshes["terrSquare"], shaders["VertexColor"], modelMatrix);
    }
}