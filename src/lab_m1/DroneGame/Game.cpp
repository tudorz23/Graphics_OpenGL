#include "lab_m1/DroneGame/Game.h"

#include "lab_m1/DroneGame/mesh_objects.h"
#include "lab_m1/DroneGame/game_constants.h"
#include "lab_m1/DroneGame/transforms3D.h"


using namespace std;
using namespace drone_game;


Game::Game()
{
}


Game::~Game()
{
}


void Game::Init()
{
    camera = new Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    Mesh* paral = objects3d::CreateParallelepiped("paral", 1, 0.5, 
											0.5, glm::vec3(0.5f, 0.5f, 0.5f));
    AddMeshToList(paral);



    Mesh* droneBodyMesh = objects3d::CreateParallelepiped("droneBody", DRONE_BODY_LEN, DRONE_BODY_WIDTH,
													DRONE_BODY_HEIGHT, COLOR_GREY);
    AddMeshToList(droneBodyMesh);

    Mesh* droneCubeMesh = objects3d::CreateParallelepiped("droneCube", DRONE_CUBE_DIM, DRONE_CUBE_DIM,
														DRONE_CUBE_DIM, COLOR_RED);
    AddMeshToList(droneCubeMesh);

    Mesh* propellerMesh = objects3d::CreateParallelepiped("propeller", DRONE_PROP_LEN, DRONE_PROP_WIDTH,
															  DRONE_PROP_HEIGHT, COLOR_GREEN);
    AddMeshToList(propellerMesh);

    this->drone = new Drone(DRONE_START_POS, 0, DRONE_BAR_INCL1, DRONE_BAR_INCL2);



    // Perspective projection params.
    fov = 60;
    aspectRatio = window->props.aspectRatio;
    persp_zNear = 0.01f;
    persp_zFar = 200.0f;

    projectionMatrix = glm::perspective(RADIANS(fov), aspectRatio, persp_zNear, persp_zFar);
}


void Game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);


    drone->resetModelMatrix();
}


void Game::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-10, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 3));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }


    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    /*{
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }*/



    drone->updatePropellerAngle(deltaTimeSeconds);

    // TEST DRONE.
    drone->prepareForRender();
    DrawDrone();
}


void Game::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Game::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}







void Game::DrawDrone()
{
	// Render bars.
    RenderMesh(meshes["droneBody"], shaders["VertexColor"], drone->bar1->modelMatrix);
    RenderMesh(meshes["droneBody"], shaders["VertexColor"], drone->bar2->modelMatrix);

    // Render cubes.
    RenderMesh(meshes["droneCube"], shaders["VertexColor"], drone->bar1->attachment1->cubeMatrix);
    RenderMesh(meshes["droneCube"], shaders["VertexColor"], drone->bar1->attachment2->cubeMatrix);
    RenderMesh(meshes["droneCube"], shaders["VertexColor"], drone->bar2->attachment1->cubeMatrix);
    RenderMesh(meshes["droneCube"], shaders["VertexColor"], drone->bar2->attachment2->cubeMatrix);

    // Render propellers.
    RenderMesh(meshes["propeller"], shaders["VertexColor"], drone->bar1->attachment1->propellerMatrix);
    RenderMesh(meshes["propeller"], shaders["VertexColor"], drone->bar1->attachment2->propellerMatrix);
    RenderMesh(meshes["propeller"], shaders["VertexColor"], drone->bar2->attachment1->propellerMatrix);
    RenderMesh(meshes["propeller"], shaders["VertexColor"], drone->bar2->attachment2->propellerMatrix);
}
 


/* Callback functions */

void Game::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->MoveForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->MoveForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        drone->position.x += DRONE_SPEED * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        drone->position.x -= DRONE_SPEED * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_UP)) {
        drone->position.z -= DRONE_SPEED * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_DOWN)) {
        drone->position.z += DRONE_SPEED * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_U)) {
        drone->position.y += DRONE_SPEED * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_I)) {
        drone->position.y -= DRONE_SPEED * deltaTime;
    }
}


void Game::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Game::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        // TODO(student): Rotate the camera in third-person mode around
        // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
        // variables for setting up the rotation speed.
        camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
        camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
    }

    //if (window->KeyHold(GLFW_MOUSE_BUTTON_LEFT)) {
        drone->rotationAngle -= DRONE_SENSITIVITY_OX * deltaX;
    //}
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Game::OnWindowResize(int width, int height)
{
}
