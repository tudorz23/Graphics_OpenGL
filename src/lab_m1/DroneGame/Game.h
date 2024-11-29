#pragma once

#include "components/simple_scene.h"
#include "lab_m1/DroneGame/camera.h"
#include "lab_m1/DroneGame/Drone.h"

#include "lab_m1/DroneGame/Tree.h"
#include "lab_m1/DroneGame/House.h"


namespace drone_game
{
	class Game final : public gfxc::SimpleScene
	{
	 public:
		 Game();
		 ~Game();

		 void Init() override;

	 private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void RenderTerrainMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
							   glm::vec3 color1, glm::vec3 color2);


        /* Game logic methods. */
        void PlaceObstacles();


        bool CheckDroneTreeCollision(glm::vec3 &dronePos);

        bool SphereIntersectsCylinder(glm::vec3 &sphereCenter, float sphereRadius,
									glm::vec3 &cylCenter, float cylRadius, float cylHeight);

        bool SphereIntersectsCone(glm::vec3& sphereCenter, float sphereRadius,
								glm::vec3& coneTipPos, float coneHeight, float coneSlope);


        /* Drawing methods. */
        void DrawDrone();

        void DrawTree(Tree* tree);

        void DrawHouse(House* house);


        /* Callback methods. */
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


	 private:
        Camera* camera;
        glm::mat4 projectionMatrix;

        // Perspective projection params.
        float fov;
        float aspectRatio;
        float persp_zNear;
        float persp_zFar;


        // Game logic objects.
        Drone* drone;


        // Trees.
        Tree* tree1;
        Tree* tree2;


        // Obstacles.
        std::vector<Tree*> trees;
        std::vector<House*> houses;


        glm::mat4 modelMatrix;
	};
}
