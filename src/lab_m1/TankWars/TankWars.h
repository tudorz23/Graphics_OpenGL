#pragma once

#include "components/simple_scene.h"

#include "lab_m1/TankWars/Terrain.h"
#include "lab_m1/TankWars/Tank.h"


namespace tw
{
    class TankWars : public gfxc::SimpleScene
    {
     public:
        TankWars();
        ~TankWars();

        void Init() override;

     private:
        // Frame handling functions.
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;


        // Game functions.
        void ApplyTransformationsToTank(Tank* tank);

        void DrawTerrain();
        void DrawTank(Tank* tank);


        // Callback functions.
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


     protected:
        glm::mat3 modelMatrix;

        Terrain *terrain;

        Tank* tank1;
        Tank* tank2;

    };
}   // namespace tw
