#pragma once

#include "components/simple_scene.h"
#include "lab_m1/DroneGame/camera.h"
#include "lab_m1/DroneGame/Drone.h"


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


        /* Game logic methods. */



        /* Drawing methods. */
        void DrawDrone();


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
         Drone* drone;





        Camera* camera;
        glm::mat4 projectionMatrix;

        // TODO(student): If you need any other class variables, define them here.
        // Perspective projection params.
        float fov;
        float aspectRatio;
        float persp_zNear;
        float persp_zFar;
	};
}
