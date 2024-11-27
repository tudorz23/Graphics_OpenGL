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
    // Initialize camera.
    camera = new Camera(FORWARD_DISTANCE, UP_DISTANCE);
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


    // Create the shader program used for terrain.
    Shader* shader = new Shader("TerrainShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneGame", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "DroneGame", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;


    // Create terrain mesh.
    Mesh* terrainMesh = objects3d::CreateTerrain("terrain", 200, 200, 0.6f, 0.4f, COLOR_PURPLE);
    AddMeshToList(terrainMesh);


    // Create drone meshes.
    Mesh* droneBodyMesh = objects3d::CreateParallelepiped("droneBody", DRONE_BODY_LEN, DRONE_BODY_WIDTH,
															DRONE_BODY_HEIGHT, COLOR_GREY);
    AddMeshToList(droneBodyMesh);

    Mesh* droneCubeMesh = objects3d::CreateParallelepiped("droneCube", DRONE_CUBE_DIM, DRONE_CUBE_DIM,
														DRONE_CUBE_DIM, COLOR_RED);
    AddMeshToList(droneCubeMesh);

    Mesh* propellerMesh = objects3d::CreateParallelepiped("propeller", DRONE_PROP_LEN, DRONE_PROP_WIDTH,
															  DRONE_PROP_HEIGHT, COLOR_GREEN);
    AddMeshToList(propellerMesh);

    this->drone = new Drone(camera->GetTargetPosition(), 0, 
							DRONE_BAR_INCL1, DRONE_BAR_INCL2);


    // Add meshes for tree.
    Mesh* cylinderMesh = objects3d::CreateCylinder("cylinder", CYLINDER_RADIUS, CYLINDER_HEIGHT, NUM_SLICES,
													COLOR_BROWN);
    AddMeshToList(cylinderMesh);

    Mesh* coneMesh = objects3d::CreateCone("cone", CONE_RADIUS, CONE_HEIGHT, NUM_SLICES, COLOR_DARK_GREEN);
    AddMeshToList(coneMesh);



    // Initialize perspective projection params.
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


    // Draw terrain.
    modelMatrix = glm::mat4(1);
    RenderTerrainMesh(meshes["terrain"], shaders["TerrainShader"], modelMatrix, COLOR_DARK_BLUE, COLOR_DARK_YELLOW);


    // TEST cylinder.
    modelMatrix = glm::mat4(1);
    modelMatrix *= transf::Translate(2, CYLINDER_HEIGHT / 2, 2);
    //modelMatrix *= transf::Scale(0.5, 1, 1);
    RenderMesh(meshes["cylinder"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transf::Translate(2, CONE_HEIGHT / 4 + CYLINDER_HEIGHT, 2);
    //modelMatrix *= transf::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["cone"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transf::Translate(2, CONE_HEIGHT / 2  + CYLINDER_HEIGHT, 2);
    //modelMatrix *= transf::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["cone"], shaders["VertexColor"], modelMatrix);
     

    drone->updatePropellerAngle(deltaTimeSeconds);

    // Update drone position based on camera position (the drone is the "target" of the camera).
    drone->position = camera->GetTargetPosition();

    // Move the drone to its current position (executed before moving it relatively to the camera).
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


void Game::RenderTerrainMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
							 glm::vec3 color1, glm::vec3 color2)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model".
    int location1 = glGetUniformLocation(shader->program, "Model");

    // Set shader uniform "Model" to modelMatrix.
    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View".
    int location2 = glGetUniformLocation(shader->program, "View");

    // Set shader uniform "View" to viewMatrix.
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection".
    int location3 = glGetUniformLocation(shader->program, "Projection");

    // Set shader uniform "Projection" to projectionMatrix.
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Get shader location for uniform vec3 "Color1".
    int location4 = glGetUniformLocation(shader->program, "Color1");

    // Set shader uniform "Color1" to color1.
    glUniform3fv(location4, 1, glm::value_ptr(color1));

    // Get shader location for uniform vec3 "Color2".
    int location5 = glGetUniformLocation(shader->program, "Color2");

    // Set shader uniform "Color1" to color1.
    glUniform3fv(location5, 1, glm::value_ptr(color2));


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}




/* Draw methods */
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
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->KeyHold(GLFW_KEY_W)) {
            // Translate the camera forward
            camera->MoveForward(DRONE_SPEED * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // Translate the camera to the left
            camera->TranslateRight(-DRONE_SPEED * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // Translate the camera backward
            camera->MoveForward(-DRONE_SPEED * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // Translate the camera to the right
            camera->TranslateRight(DRONE_SPEED * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // Translate the camera downward
            camera->MoveUpward(-DRONE_SPEED * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // Translate the camera upward
            camera->MoveUpward(DRONE_SPEED * deltaTime);
        }


        if (window->KeyHold(GLFW_KEY_Z))
        {
            drone->rotationAngle += DRONE_ROTATION_SPEED * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_X))
        {
            drone->rotationAngle -= DRONE_ROTATION_SPEED * deltaTime;
        }
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
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        camera->RotateThirdPerson_OX(-SENSITIVITY_OX * deltaY);
        camera->RotateThirdPerson_OY(-SENSITIVITY_OY * deltaX);

        drone->rotationAngle -= SENSITIVITY_OX * deltaX;
    }
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
