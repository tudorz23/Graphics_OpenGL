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

    this->LIFE_RECT_LEN = 0.0f;

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


    // Add meshes for tank1.
    Mesh* tankBodyMesh1 = objects::CreateTankBody("body1", COLOR_BLACK, COLOR_PURPLE);
    AddMeshToList(tankBodyMesh1);

    Mesh* tankHeadMesh1 = objects::CreateSemiCircle("head1", TANK_HEAD_RADIUS, COLOR_BLACK);
    AddMeshToList(tankHeadMesh1);

    Mesh* tankPipeMesh1 = objects::CreatePipe("pipe1", TANK_PIPE_WIDTH, TANK_PIPE_LENGTH, COLOR_BLACK);
    AddMeshToList(tankPipeMesh1);

    Mesh* tankBarMesh1 = objects::CreateLifeBar("bar1", BAR_LEN, BAR_WIDTH, BAR_THICK, COLOR_BLACK);
    AddMeshToList(tankBarMesh1);

    this->tank1 = new Tank("body1", "head1", "pipe1", "bar1",
                            START_X1, START_Y1, SPEED1, ROTATE_SPEED);

    // Add meshes for tank2.
    Mesh* tankBodyMesh2 = objects::CreateTankBody("body2", COLOR_RED, COLOR_YELLOW);
    AddMeshToList(tankBodyMesh2);

    Mesh* tankHeadMesh2 = objects::CreateSemiCircle("head2", TANK_HEAD_RADIUS, COLOR_RED);
    AddMeshToList(tankHeadMesh2);

    Mesh* tankPipeMesh2 = objects::CreatePipe("pipe2", TANK_PIPE_WIDTH, TANK_PIPE_LENGTH, COLOR_BLACK);
    AddMeshToList(tankPipeMesh2);

    Mesh* tankBarMesh2 = objects::CreateLifeBar("bar2", BAR_LEN, BAR_WIDTH, BAR_THICK, COLOR_BLACK);
    AddMeshToList(tankBarMesh2);

    this->tank2 = new Tank("body2", "head2", "pipe2", "bar2",
                            START_X2, START_Y2, SPEED2, ROTATE_SPEED);

    tank1->orientate(terrain->heightMap);
    tank2->orientate(terrain->heightMap);

    
    // Add a rectangle mesh for lives.
    this->LIFE_RECT_LEN = BAR_LEN / INITIAL_LIVES;
    Mesh* lifeRectangle = objects::CreatePipe("lifeRectangle", LIFE_RECT_LEN, BAR_WIDTH, COLOR_DARK_GREEN);
    AddMeshToList(lifeRectangle);


    // Add the missile mesh.
    Mesh* missileMesh = objects::CreateCircle("missile", MISSILE_RADIUS, COLOR_BLACK);
    AddMeshToList(missileMesh);

    // Add trajectory guidance circle mesh.
    Mesh* trajMesh = objects::CreateCircle("traj", TRAJ_RADIUS, COLOR_WHITE);
    AddMeshToList(trajMesh);

    tank1->computeTrajectory(resolution.x);
    tank2->computeTrajectory(resolution.x);
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


    // Update the positions of the missiles.
    for (Missile* missile : this->missiles) {
        if (missile->active) {
            missile->UpdatePosition(deltaTimeSeconds);
        }
    }

    CheckMissileTankCollisions();

    // Remove inactive missiles.
    RemoveInactiveMissiles();

    /*if (missiles.empty()) {
        std::cout << "Lewis Hamilton\n";
    }*/


    // Draw all the scene components.
    DrawTank(tank1);
    DrawTank(tank2);

    DrawTerrain();

    DrawMissiles();

    DrawTrajectory(tank1);
    DrawTrajectory(tank2);
}


void TankWars::FrameEnd()
{
}


void TankWars::ApplyTransformationsToTank(Tank* tank)
{
    if (!tank->isAlive()) {
        return;
    }

    // Rotate the tank with slopeAngle.
    // Translate the tank to its current position.
    // Affected: body, head, pipe.
    tank->translate(tank->posX, tank->posY);
    tank->rotate(tank->slopeAngle);

    // Translate the head to the top of the tank (this will be done first, so it
    // rotates around the tank's center).
    // Affected: head.
    tank->headMatrix *= transform::Translate(0, TANK_TOTAL_HEIGHT);

    // Rotate the pipe around its own center, then translate it
    // to the head's center position.
    // Affected: pipe.
    tank->pipeMatrix *= transform::Translate(tank->pipeX, tank->pipeY);
    tank->pipeMatrix *= transform::Rotate(tank->pipeAngle);

    // Translate the life bar above the tank.
    tank->barMatrix *= transform::Translate(tank->posX, tank->posY + BAR_DELTA);
}


void TankWars::CheckMissileTankCollisions()
{
    // Check for collisions between missiles and the tanks.
    for (Missile* missile : this->missiles) {
        if (!missile->active) {
            continue;
        }

        // Launched by 1, collision with 2.
        if (missile->launcher == 1 && tank2->isAlive()) {
            if (CirclesCollide(missile->radius, missile->posX, missile->posY,
                               TANK_RADIUS, tank2->posX, tank2->posY)) {
                tank2->decrementLives();
                missile->active = false;
            }
        }

        // Launched by 2, collision with 1.
        if (missile->launcher == 2 && tank1->isAlive()) {
            if (CirclesCollide(missile->radius, missile->posX, missile->posY,
                               TANK_RADIUS, tank1->posX, tank1->posY)) {
                tank1->decrementLives();
                missile->active = false;
            }
        }
    }
}


bool TankWars::CirclesCollide(float radius1, float x1, float y1,
                              float radius2, float x2, float y2)
{
    float a = (radius1 - radius2) * (radius1 - radius2);
    float b = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    float c = (radius1 + radius2) * (radius1 + radius2);

    return (a <= b && b <= c);
}


void TankWars::RemoveInactiveMissiles()
{
    vector<Missile*>::iterator it = this->missiles.begin();

    while (it != this->missiles.end()) {
        Missile* missile = *it;
        if (!missile->active) {
            it = this->missiles.erase(it);
            delete missile;
        }
        else {
            it++;
        }
    }
}


/* Draw methods */

void TankWars::DrawTerrain()
{
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["terrain"], shaders["VertexColor"], modelMatrix);
}


void TankWars::DrawTank(Tank* tank)
{
    if (!tank->isAlive()) {
        return;
    }

    // Render body.
    RenderMesh2D(meshes[tank->bodyName], shaders["VertexColor"], tank->bodyMatrix);
    
    // Render head.
    RenderMesh2D(meshes[tank->headName], shaders["VertexColor"], tank->headMatrix);

    // Render pipe.
    RenderMesh2D(meshes[tank->pipeName], shaders["VertexColor"], tank->pipeMatrix);

    // Render life bar.
    RenderMesh2D(meshes[tank->barName], shaders["VertexColor"], tank->barMatrix);

    // Render life rectangles.
    float firstX = tank->posX - BAR_LEN / 2.0f + LIFE_RECT_LEN / 2.0f;
    float y = tank->posY + BAR_DELTA + BAR_THICK;
    for (int i = 0; i < tank->lives; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(firstX + i * LIFE_RECT_LEN, y);
        RenderMesh2D(meshes["lifeRectangle"], shaders["VertexColor"], modelMatrix);
    }
}


void TankWars::DrawMissiles()
{
    for (Missile* missile : this->missiles) {
        if (missile->active) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform::Translate(missile->posX, missile->posY);
            RenderMesh2D(meshes["missile"], shaders["VertexColor"], modelMatrix);
        }
    }
}


void TankWars::DrawTrajectory(Tank* tank)
{
    if (!tank->isAlive()) {
        return;
    }

    // Render trajectory guidance.
    for (const auto& point : tank->trajectory) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform::Translate(point.first, point.second);
        RenderMesh2D(meshes["traj"], shaders["VertexColor"], modelMatrix);
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void TankWars::OnInputUpdate(float deltaTime, int mods)
{
    // Move first tank.
    if (window->KeyHold(GLFW_KEY_A) && tank1->isAlive()) {
        if (tank1->posX > LEFT_LIMIT) {
            tank1->posX -= tank1->moveSpeed * deltaTime;
            tank1->orientate(terrain->heightMap);

            glm::ivec2 resolution = window->GetResolution();
            tank1->computeTrajectory((float)resolution.x);
        }
    }

    if (window->KeyHold(GLFW_KEY_D) && tank1->isAlive()) {
        if (tank1->posX < RIGHT_LIMIT) {
            tank1->posX += tank1->moveSpeed * deltaTime;
            tank1->orientate(terrain->heightMap);

            glm::ivec2 resolution = window->GetResolution();
            tank1->computeTrajectory((float)resolution.x);
        }
    }

    // Rotate first tank's pipe.
    if (window->KeyHold(GLFW_KEY_W) && tank1->isAlive()) {
        tank1->pipeAngle -= tank1->pipeRotationSpeed * deltaTime;

        glm::ivec2 resolution = window->GetResolution();
        tank1->computeTrajectory((float) resolution.x);
    }

    if (window->KeyHold(GLFW_KEY_S) && tank1->isAlive()) {
        tank1->pipeAngle += tank1->pipeRotationSpeed * deltaTime;

        glm::ivec2 resolution = window->GetResolution();
        tank1->computeTrajectory((float)resolution.x);
    }

    // Move second tank.
    if (window->KeyHold(GLFW_KEY_LEFT) && tank2->isAlive()) {
        if (tank2->posX > LEFT_LIMIT) {
            tank2->posX -= tank2->moveSpeed * deltaTime;
            tank2->orientate(terrain->heightMap);
        }

        glm::ivec2 resolution = window->GetResolution();
        tank2->computeTrajectory((float)resolution.x);
    }

    if (window->KeyHold(GLFW_KEY_RIGHT) && tank2->isAlive()) {
        if (tank2->posX < RIGHT_LIMIT) {
            tank2->posX += tank2->moveSpeed * deltaTime;
            tank2->orientate(terrain->heightMap);
        }

        glm::ivec2 resolution = window->GetResolution();
        tank2->computeTrajectory((float)resolution.x);
    }

    // Rotate second tank's pipe.
    if (window->KeyHold(GLFW_KEY_UP) && tank2->isAlive()) {
        tank2->pipeAngle -= tank2->pipeRotationSpeed * deltaTime;

        glm::ivec2 resolution = window->GetResolution();
        tank2->computeTrajectory((float)resolution.x);
    }

    if (window->KeyHold(GLFW_KEY_DOWN) && tank2->isAlive()) {
        tank2->pipeAngle += tank2->pipeRotationSpeed * deltaTime;

        glm::ivec2 resolution = window->GetResolution();
        tank2->computeTrajectory((float)resolution.x);
    }
}


void TankWars::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_G) {
        tank1->lives = INITIAL_LIVES;
    }

    if (key == GLFW_KEY_H) {
        tank2->lives = INITIAL_LIVES;
    }

    // Launch missile from tank1.
    if (key == GLFW_KEY_SPACE && tank1->isAlive()) {
        float pipeHeadX = tank1->posX + tank1->pipeX - glm::sin(tank1->pipeAngle) * TANK_PIPE_LENGTH;
        float pipeHeadY = tank1->posY + tank1->pipeY + glm::cos(tank1->pipeAngle) * TANK_PIPE_LENGTH;

        glm::ivec2 resolution = window->GetResolution();

        Missile* missile = new Missile(pipeHeadX, pipeHeadY, tank1->pipeAngle, MISSILE_POW,
                                       GRAVITY, (float) resolution.x, MISSILE_RADIUS, 1);
        missiles.push_back(missile);

        cout << "pipeAngle = " << tank1->pipeAngle << "\n";
        cout << "tank pipe x = " << tank1->pipeX << ", y = " << tank1->pipeY << "\n";
        cout << "Missile here, x = " << missile->posX << ", y = " << missile->posY << "\n\n";
    }

    // Launch missile from tank2.
    if (key == GLFW_KEY_ENTER && tank2->isAlive()) {
        float pipeHeadX = tank2->posX + tank2->pipeX - glm::sin(tank2->pipeAngle) * TANK_PIPE_LENGTH;
        float pipeHeadY = tank2->posY + tank2->pipeY + glm::cos(tank2->pipeAngle) * TANK_PIPE_LENGTH;

        glm::ivec2 resolution = window->GetResolution();

        Missile* missile = new Missile(pipeHeadX, pipeHeadY, tank2->pipeAngle, MISSILE_POW,
                                       GRAVITY, (float)resolution.x, MISSILE_RADIUS, 2);
        missiles.push_back(missile);

        cout << "pipeAngle = " << tank2->pipeAngle << "\n";
        cout << "tank pipe x = " << tank2->pipeX << ", y = " << tank2->pipeY << "\n";
        cout << "Missile here, x = " << missile->posX << ", y = " << missile->posY << "\n\n";
    }
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
