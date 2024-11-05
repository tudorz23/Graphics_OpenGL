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
    this->terrain->computeHeights(0.0, (float)resolution.x, TERRAIN_POINT_INTERV);


    // Add the terrain mesh.
    Mesh* terrainMesh = objects::CreateTerrain("terrain", TERRAIN_COLOR, terrain->heightMap);
    AddMeshToList(terrainMesh);


    // Add meshes for tank1.
    Mesh* tankBodyMesh1 = objects::CreateTankBody("body1", COLOR_BLACK, COLOR_PURPLE);
    AddMeshToList(tankBodyMesh1);

    Mesh* tankHeadMesh1 = objects::CreateSemiCircle("head1", TANK_HEAD_RADIUS, COLOR_BLACK);
    AddMeshToList(tankHeadMesh1);

    Mesh* tankPipeMesh1 = objects::CreateRectangle("pipe1", TANK_PIPE_WIDTH, TANK_PIPE_LENGTH, COLOR_BLACK);
    AddMeshToList(tankPipeMesh1);

    this->tank1 = new Tank("body1", "head1", "pipe1",
                            START_X1, START_Y1, SPEED1, ROTATE_SPEED);

    // Add meshes for tank2.
    Mesh* tankBodyMesh2 = objects::CreateTankBody("body2", COLOR_RED, COLOR_YELLOW);
    AddMeshToList(tankBodyMesh2);

    Mesh* tankHeadMesh2 = objects::CreateSemiCircle("head2", TANK_HEAD_RADIUS, COLOR_RED);
    AddMeshToList(tankHeadMesh2);

    Mesh* tankPipeMesh2 = objects::CreateRectangle("pipe2", TANK_PIPE_WIDTH, TANK_PIPE_LENGTH, COLOR_BLACK);
    AddMeshToList(tankPipeMesh2);

    this->tank2 = new Tank("body2", "head2", "pipe2",
                            START_X2, START_Y2, SPEED2, ROTATE_SPEED);

    tank1->orientate(terrain->heightMap);
    tank2->orientate(terrain->heightMap);

    // Add lifeBar mesh.
    Mesh* tankBarMesh = objects::CreateLifeBar("lifeBar", BAR_LEN, BAR_WIDTH, BAR_THICK, COLOR_BLACK);
    AddMeshToList(tankBarMesh);
    
    // Add a rectangle mesh for lives.
    Mesh* lifeRectangle = objects::CreateRectangle("lifeRectangle", LIFE_RECT_LEN, BAR_WIDTH, COLOR_DARK_GREEN);
    AddMeshToList(lifeRectangle);


    // Add the missile mesh.
    Mesh* missileMesh = objects::CreateCircle("missile", MISSILE_RADIUS, COLOR_BLACK);
    AddMeshToList(missileMesh);

    // Add trajectory guidance circle mesh.
    Mesh* trajMesh = objects::CreateCircle("traj", TRAJ_RADIUS, COLOR_WHITE);
    AddMeshToList(trajMesh);
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
    // Update the positions of the missiles.
    for (Missile* missile : this->missiles) {
        if (missile->active) {
            missile->UpdatePosition(deltaTimeSeconds);
        }
    }

    CheckMissileTankCollisions();

    CheckMissileTerrainCollisions();

    // Remove inactive missiles.
    RemoveInactiveMissiles();

    CheckTerrainSlide(deltaTimeSeconds);

    if (terrain->hasChanged) {
        Mesh* oldTerrainMesh = meshes["terrain"];
        //delete oldTerrainMesh;
        
        Mesh* newTerrainMesh = objects::CreateTerrain("terrain", TERRAIN_COLOR, terrain->heightMap);
        AddMeshToList(newTerrainMesh);

        tank1->orientate(terrain->heightMap);
        tank2->orientate(terrain->heightMap);
    }

    // Update the slopes of the tanks gradually at each frame, so there is no lag.
    tank1->updateOrientation(deltaTimeSeconds);
    tank2->updateOrientation(deltaTimeSeconds);

    // Prepare the model matrixes of the tanks.
    ApplyTransformationsToTank(tank1);
    ApplyTransformationsToTank(tank2);

    if (tank1->hasMoved) {
        tank1->computeTrajectory((float)window->GetResolution().x);
    }

    if (tank2->hasMoved) {
        tank2->computeTrajectory((float)window->GetResolution().x);
    }


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
    // Reset these at the end of the frame, because OnInputUpdate() is called
    // before FrameStart(), and these variables can be altered there.
    tank1->hasMoved = false;
    tank2->hasMoved = false;

    terrain->hasChanged = false;
}


void TankWars::ApplyTransformationsToTank(Tank* tank)
{
    if (!tank->isAlive()) {
        return;
    }

    // Rotate the tank with slopeAngle.
    // Translate the tank to its current position.
    // Affected: body, head, pipe, bar.
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
    tank->barMatrix *= transform::Translate(0, BAR_DELTA);
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
                               TANK_RADIUS, tank2->posX, tank2->posY + TANK_LOWER_HEIGHT)) {
                tank2->decrementLives();
                missile->MarkInactive();
            }
        }

        // Launched by 2, collision with 1.
        if (missile->launcher == 2 && tank1->isAlive()) {
            if (CirclesCollide(missile->radius, missile->posX, missile->posY,
                               TANK_RADIUS, tank1->posX, tank1->posY + TANK_LOWER_HEIGHT)) {
                tank1->decrementLives();
                missile->MarkInactive();
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



void TankWars::CheckMissileTerrainCollisions()
{
    for (Missile* missile : this->missiles) {
        if (!missile->active) {
            continue;
        }

        // Get the index of the point from terrain->heightMap that has
        // the closest x coord to the left to the x coord of the missile.
        int index = (int) (missile->posX / TERRAIN_POINT_INTERV);

        if (index + 1 == this->terrain->heightMap.size()) {
            continue;
        }

        pair<float, float>& point1 = this->terrain->heightMap[index];
        pair<float, float>& point2 = this->terrain->heightMap[index + 1];

        // Get interpolation coefficient.
        float t = (missile->posX - point1.first) / TERRAIN_POINT_INTERV;

        // Get coords of the projection of the missile on the terrain.
        float projX = point1.first * (1.0f - t) + point2.first * t;
        float projY = point1.second * (1.0f - t) + point2.second * t;

        if (glm::abs(missile->posY - projY) <= MISSILE_RADIUS) {

            // Debug stuff.
            std::cout << "Collision at index: " << index << "\n";
            std::cout << "Interpolation coef = " << t << "\n";
            std::cout << "point1.x = " << point1.first << ", point1.y = " << point1.second << "\n";
            std::cout << "point2.x = " << point2.first << ", point2.y = " << point2.second << "\n";
            std::cout << "Collision at missile.y = " << missile->posY << ", and projY = " << projY << "\n";
            std::cout << "Collision at missile.x = " << missile->posX << ", and projX = " << projX << "\n\n";

            DeformTerrainCircular(missile->posX, missile->posY, EXPLOSION_RADIUS);
            missile->MarkInactive();
        }
    }
}


void TankWars::DeformTerrainCircular(float centerX, float centerY, float radius)
{
    // Get the leftmost and rightmost affected indexes from the heightMap,
    // then decrease the y's of their attached points.
    int leftIndex = (int) ((centerX - radius) / TERRAIN_POINT_INTERV);
    leftIndex = max(0, leftIndex);

    int rightIndex = (int) ((centerX + radius) / TERRAIN_POINT_INTERV);
    rightIndex = min((int) (terrain->heightMap.size() - 1), rightIndex);

    // Start from leftIndex + 1, because leftIndex is out of circle scope.
    for (int idx = leftIndex + 1; idx <= rightIndex; idx++) {
        float x = terrain->heightMap[idx].first;

        // Eq. (x - centerX) ^ 2 + (y - centerY) ^ 2 = radius ^ 2
        // Two values for Y will be obtained, choose the smaller one.
        // y = -sqrt(radius ^ 2 - (x - centerX) ^ 2) + centerY
        float deltaX = x - centerX;
        float y = -glm::sqrt(radius * radius - deltaX * deltaX) + centerY;

        // Check that terrain is not somehow "added" (encountered this in testing).
        if (y > terrain->heightMap[idx].second) {
            continue;
        }

        terrain->heightMap[idx] = { x, max(0.0f, y) };
    }

    terrain->hasChanged = true;
}


void TankWars::CheckTerrainSlide(float deltaTime)
{
    for (int i = 0; i < this->terrain->heightMap.size() - 1; i++) {
        auto& point1 = this->terrain->heightMap[i];
        auto& point2 = this->terrain->heightMap[i + 1];

        if (glm::abs(point1.second - point2.second) < TERRAIN_SLIDE_LIMIT) {
            continue;
        }

        int minIdx = -1;
        int maxIdx = -1;

        if (point1.second <= point2.second) {
            minIdx = i;
            maxIdx = i + 1;
        } else {
            minIdx = i + 1;
            maxIdx = i;
        }

        terrain->heightMap[minIdx].second += TERRAIN_SLIDE_EPSILON * deltaTime;
        terrain->heightMap[maxIdx].second -= TERRAIN_SLIDE_EPSILON * deltaTime;

        terrain->hasChanged = true;
    }
}



void TankWars::RemoveInactiveMissiles()
{
    if (this->missiles.empty()) {
        return;
    }

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
    RenderMesh2D(meshes["lifeBar"], shaders["VertexColor"], tank->barMatrix);

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
        }
    }

    if (window->KeyHold(GLFW_KEY_D) && tank1->isAlive()) {
        if (tank1->posX < RIGHT_LIMIT) {
            tank1->posX += tank1->moveSpeed * deltaTime;
            tank1->orientate(terrain->heightMap);
        }
    }

    // Rotate first tank's pipe.
    if (window->KeyHold(GLFW_KEY_W) && tank1->isAlive()) {
        tank1->pipeAngle -= tank1->pipeRotationSpeed * deltaTime;

        tank1->hasMoved = true;
    }

    if (window->KeyHold(GLFW_KEY_S) && tank1->isAlive()) {
        tank1->pipeAngle += tank1->pipeRotationSpeed * deltaTime;

        tank1->hasMoved = true;
    }

    // Move second tank.
    if (window->KeyHold(GLFW_KEY_LEFT) && tank2->isAlive()) {
        if (tank2->posX > LEFT_LIMIT) {
            tank2->posX -= tank2->moveSpeed * deltaTime;
            tank2->orientate(terrain->heightMap);
        }
    }

    if (window->KeyHold(GLFW_KEY_RIGHT) && tank2->isAlive()) {
        if (tank2->posX < RIGHT_LIMIT) {
            tank2->posX += tank2->moveSpeed * deltaTime;
            tank2->orientate(terrain->heightMap);
        }
    }

    // Rotate second tank's pipe.
    if (window->KeyHold(GLFW_KEY_UP) && tank2->isAlive()) {
        tank2->pipeAngle -= tank2->pipeRotationSpeed * deltaTime;

        tank2->hasMoved = true;
    }

    if (window->KeyHold(GLFW_KEY_DOWN) && tank2->isAlive()) {
        tank2->pipeAngle += tank2->pipeRotationSpeed * deltaTime;

        tank2->hasMoved = true;
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
        pair<float, float> pipeHeadPos = tank1->getPipeHeadPos();
        glm::ivec2 resolution = window->GetResolution();

        Missile* missile = new Missile(pipeHeadPos.first, pipeHeadPos.second, tank1->pipeAngle,
                                       MISSILE_POW, GRAVITY, (float) resolution.x, MISSILE_RADIUS, 1);
        missiles.push_back(missile);
    }

    // Launch missile from tank2.
    if (key == GLFW_KEY_ENTER && tank2->isAlive()) {
        pair<float, float> pipeHeadPos = tank2->getPipeHeadPos();
        glm::ivec2 resolution = window->GetResolution();

        Missile* missile = new Missile(pipeHeadPos.first, pipeHeadPos.second, tank2->pipeAngle,
                                       MISSILE_POW, GRAVITY, (float)resolution.x, MISSILE_RADIUS, 2);
        missiles.push_back(missile);
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
