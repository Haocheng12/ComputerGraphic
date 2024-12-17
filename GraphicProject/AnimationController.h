#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Mesh.h"
#include "Camera.h"
#include "Collison.h"
class AnimationController {
public:
    float speed = 3.5f;

    void update(AnimationModel& trex, Camera& camera, DirectX::XMMATRIX& trexWorld, float dt, std::vector<AABB>& Colliders) {
        // Extract the translation (position) from the world matrix directly
        DirectX::XMFLOAT3 trexPosition;
        trexPosition.x = trexWorld.r[3].m128_f32[0];  // X component from the 4th row, 1st column of the matrix
        trexPosition.y = trexWorld.r[3].m128_f32[1];  // Y component from the 4th row, 2nd column of the matrix
        trexPosition.z = trexWorld.r[3].m128_f32[2];  // Z component from the 4th row, 3rd column of the matrix

        // Calculate the distance between camera and T-Rex in 2D (x, z plane)
        float dx = trexPosition.x - camera.position.x;
        float dz = trexPosition.z - camera.position.z;
        float distance = sqrtf(dx * dx + dz * dz);

        // Calculate direction to the camera
        DirectX::XMFLOAT3 directionToCamera = normalizeDirection(trexPosition, camera.position);
        float angle = std::atan2(directionToCamera.x, directionToCamera.z);  // Calculate the angle in the XZ plane

        // Update animation based on distance
        if (distance > 30.0f && distance < 40.0f) {
            // If the camera is far away (greater than 50), play "roar"
            trex.instance.update("roar", dt);
        }
        else if (distance <= 30.0f && distance > 5.0f) {
            // If the camera is within 30 units but greater than 5, play "walk" and move the T-Rex
            trex.instance.update("walk", dt);

            // Move the T-Rex towards the camera
            trexPosition.x -= directionToCamera.x * speed * dt;
            trexPosition.z -= directionToCamera.z * speed * dt;

            // Rotate the T-Rex to face the camera
            angle -= DirectX::XMConvertToRadians(200.0f);  // Adjust based on model's facing direction
            DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationY(angle);  // Create a Y-axis rotation matrix

            // Combine the translation and rotation into the new world matrix
            DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(trexPosition.x, trexPosition.y, trexPosition.z);
            trexWorld = rotation * translation;

            // Update the collider's position based on the new T-Rex position
            updateCollider(Colliders[0], trexPosition);  // Update the collider for the T-Rex
        }
        else if (distance <= 20.0f) {
            // If the camera is too close (within 10), play "attack"
            trex.instance.update("attack", dt);

            // Prevent the T-Rex from getting too close, move it away from the camera
            angle -= DirectX::XMConvertToRadians(180.0f);
            DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationY(angle);
            DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(trexPosition.x, trexPosition.y, trexPosition.z);
            trexWorld = rotation * translation;

            // Update the collider's position based on the new T-Rex position
            updateCollider(Colliders[0], trexPosition);  // Update the collider for the T-Rex
        }
    }

private:
    // Helper function to normalize the direction vector from T-Rex to the camera
    DirectX::XMFLOAT3 normalizeDirection(const DirectX::XMFLOAT3& trexPosition, const DirectX::XMFLOAT3& cameraPosition) {
        float dx = trexPosition.x - cameraPosition.x;
        float dz = trexPosition.z - cameraPosition.z;
        float length = sqrtf(dx * dx + dz * dz);
        return DirectX::XMFLOAT3(dx / length, 0.0f, dz / length);  // Normalize in the x and z axes
    }

    // Helper function to update the collider's position based on the T-Rex's position
    void updateCollider(AABB& collider, const DirectX::XMFLOAT3& trexPosition) {
        // Assuming the collider's center is at the T-Rex's position, set the AABB based on that
        collider.setFromCenterAndSize(trexPosition);  // Adjust the size as needed
    }
};
