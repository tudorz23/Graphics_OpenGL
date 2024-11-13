#pragma once

#include "components/simple_scene.h"

#include "lab_m1/TankWars/Terrain.h"
#include "lab_m1/TankWars/Tank.h"
#include "lab_m1/TankWars/Missile.h"
#include "lab_m1/TankWars/Bird.h"

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
        /* Frame handling methods. */
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;


        /* Game logic methods. */
        void ApplyTransformationsToTank(Tank* tank);


        // Consider that the missile and the tank are each surrounded by a circle
        // and check if those two circles intersect.
        void CheckMissileTankCollisions();


        void CheckMissileBirdCollisions();


        void CheckMissileTargetCollisions();


        // Check if two circles intersect. Used for collision detection between
        // missiles and other objects.
        bool CirclesCollide(float radius1, float centerX1, float centerY1,
                            float radius2, float centerX2, float centerY2);


        // Uses interpolation to determine if a missile is close enough to
        // the surface of the terrain to consider that they collide.
        void CheckMissileTerrainCollisions();


        // Creates a deformation in the terrain, with the form a circle with
        // the specified params. Uses the circle equation to determine the new height
        // for every affected point.
        void DeformTerrainCircular(float centerX, float centerY, float radius);


        // Iterates the heightMap and if, for two adjacent points, the heights differ
        // by more than a limit, transfers an epsilon from the higher to the lower.
        void CheckTerrainSlide(float deltaTime);


        void RemoveInactiveMissiles();


        void UpdateTargetPosition(float deltaTime);


        /* Drawing methods. */
        void DrawTerrain();

        void DrawTank(Tank* tank);

        void DrawMissiles();

        void DrawTrajectory(Tank* tank);

        void DrawBirds();

        void DrawTarget();


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
        glm::mat3 modelMatrix;

        Terrain *terrain;

        Tank* tank1;
        Tank* tank2;

        // Collection of missiles.
        std::vector<Missile*> missiles;

        // Collection of birds (if one of them is hit, the tank loses one life).
        std::vector<Bird*> birds;

        // For the moving target (if it is hit, the tank gets an extra life).
        float targetPosX;
        float targetPosY;
        float targetSpeedX;
        float targetSpeedY;
    };
}   // namespace tw
