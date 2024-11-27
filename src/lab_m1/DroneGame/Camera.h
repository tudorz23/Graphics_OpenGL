#pragma once

#include "game_constants.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace drone_game
{
	class Camera
	{
     public:
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        float forwardDistanceToTarget;
        float upDistanceToTarget;


        // Constructor.
		Camera(float forwardDistanceToTarget, float upDistanceToTarget)
		{
			position = glm::vec3(0, 2, 5);
			forward = glm::vec3(0, 0, -1);
			up = glm::vec3(0, 1, 0);
			right = glm::vec3(1, 0, 0);

            this->forwardDistanceToTarget = forwardDistanceToTarget;
            this->upDistanceToTarget = upDistanceToTarget;
		}

        // Constructor.
        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up,
			   float forwardDistanceToTarget, float upDistanceToTarget)
        {
            Set(position, center, up);

            this->forwardDistanceToTarget = forwardDistanceToTarget;
            this->upDistanceToTarget = upDistanceToTarget;
        }


        ~Camera()
        { }


        // Update camera.
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }


        void MoveForward(float distance)
        {
            // Movement will always keep the camera at the same height.
            // For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground.
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }


        void TranslateForward(float distance)
        {
            position += forward * distance;
        }


        void TranslateUpward(float distance)
        {
            position += up * distance;
        }


        void TranslateRight(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += dir * distance;

        }


        void RotateFirstPerson_OX(float angle)
        {
            glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, right)
                * glm::vec4(forward, 1.0f);

            forward = glm::normalize(glm::vec3(newVector));
            up = glm::normalize(glm::cross(right, forward));
        }


        void RotateFirstPerson_OY(float angle)
        {
            glm::vec4 newVecForward = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0))
										* glm::vec4(forward, 1);
            forward = glm::normalize(glm::vec3(newVecForward));

            glm::vec4 newVecRight = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0))
									* glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(newVecRight));

            up = glm::normalize(glm::cross(right, forward));
        }


        void RotateFirstPerson_OZ(float angle)
        {
            glm::vec4 newVecRight = glm::rotate(glm::mat4(1.f), angle, forward)
									* glm::vec4(right, 1);
            right = glm::normalize(glm::vec3(newVecRight));

            up = glm::normalize(glm::cross(right, forward));
        }


        void RotateThirdPerson_OX(float angle)
        {
            // Translate the observer on view direction (forward) to the target.
            TranslateForward(forwardDistanceToTarget);

            RotateFirstPerson_OX(angle);

            // Translate the observer back.
            TranslateForward(-forwardDistanceToTarget);
        }


        void RotateThirdPerson_OY(float angle)
        {
            TranslateForward(forwardDistanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-forwardDistanceToTarget);
        }


        void RotateThirdPerson_OZ(float angle)
        {
            TranslateForward(forwardDistanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-forwardDistanceToTarget);
        }


        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }


        glm::vec3 GetTargetPosition()
        {
            return position + forward * forwardDistanceToTarget + up * upDistanceToTarget;
        }
	};
}
