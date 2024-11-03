#pragma once

#include "components/simple_scene.h"

#include "lab_m1/TankWars/Terrain.h"
#include "lab_m1/TankWars/Tank.h"
#include "lab_m1/TankWars/Missile.h"

#include <vector>


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


        // Game logic methods.
        void ApplyTransformationsToTank(Tank* tank);


        // Consider that the missile and the tank are each surrounded by a circle
        // and check if those two circles intersect.
        void CheckMissileTankCollisions();


        // Check if two circles intersect. Used for collision detection between
        // missiles and tanks.
        bool CirclesCollide(float radius1, float centerX1, float centerY1,
                            float radius2, float centerX2, float centerY2);


        void CheckMissileTerrainCollisions();


        void RemoveInactiveMissiles();


        // Drawing methods.
        void DrawTerrain();
        void DrawTank(Tank* tank);

        void DrawMissiles();

        void DrawTrajectory(Tank* tank);


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

        // Collection of missiles.
        std::vector<Missile*> missiles;
    };
}   // namespace tw
