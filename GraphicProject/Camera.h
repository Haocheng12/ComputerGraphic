#pragma once

#include <DirectXMath.h>
#include <windows.h>


const float CAMERAHEIGHT = 1.0f;
class Camera {
public:
    // Constructor
    Camera(float aspectRatio)
        : position(0.0f, CAMERAHEIGHT, -1.0f),    // Initial camera position
        target(0.0f, 0.0f, 0.0f),       // Initial target position
        up(0.0f, 1.0f, 0.0f),            // Up direction
        yaw(0.0f), pitch(0.0f),          // Initial rotation angles
        speed(5.0f), sensitivity(0.1f),  // Movement and mouse sensitivity
        aspectRatio(aspectRatio) {
        UpdateViewMatrix();
    }

    // Get the current view matrix
    DirectX::XMMATRIX GetViewMatrix() const {
        return viewMatrix;
    }

    // Update camera position based on input
    void Update(float deltaTime, HWND hwnd) {
        ProcessKeyboard(deltaTime);
        ProcessMouse(hwnd);
        LockMouseToWindow(hwnd);
        UpdateViewMatrix();
       
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            PostMessage(hwnd, WM_CLOSE, 0, 0);  // Post a message to close the window
        }
    }

private:
    // Camera attributes
    DirectX::XMFLOAT3 position;  // Camera position
    DirectX::XMFLOAT3 target;    // Camera target
    DirectX::XMFLOAT3 up;        // Up vector

    float yaw;                   // Horizontal rotation
    float pitch;                 // Vertical rotation
    float speed;                 // Movement speed
    float sensitivity;           // Mouse sensitivity
    float aspectRatio;           // Aspect ratio of the camera

    DirectX::XMMATRIX viewMatrix; // Cached view matrix

    // Update the view matrix
    void UpdateViewMatrix() {
        // Convert yaw and pitch to direction vector
        DirectX::XMFLOAT3 direction;
        direction.x = cosf(DirectX::XMConvertToRadians(yaw)) * cosf(DirectX::XMConvertToRadians(pitch));
        direction.y = sinf(DirectX::XMConvertToRadians(pitch));
        direction.z = sinf(DirectX::XMConvertToRadians(yaw)) * cosf(DirectX::XMConvertToRadians(pitch));

        DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));
        DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&up);

        // Calculate target
        DirectX::XMStoreFloat3(&target, DirectX::XMVectorAdd(pos, dir));

        // Update the view matrix
        viewMatrix = DirectX::XMMatrixLookAtLH(pos, DirectX::XMLoadFloat3(&target), upVec);
    }

    // Handle keyboard input
    void ProcessKeyboard(float deltaTime) {
        float velocity = speed * deltaTime;

        // WASD controls
        if (GetAsyncKeyState('W') & 0x8000) {
            MoveForward(velocity);
        }
        if (GetAsyncKeyState('S') & 0x8000) {
            MoveForward(-velocity);
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            Strafe(-velocity);
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            Strafe(velocity);
        }
    }

    // Handle mouse input
    void ProcessMouse(HWND hwnd) {
        // Get the current position of the cursor
        POINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(hwnd, &cursorPos);  // Convert to client coordinates

        RECT windowRect;
        GetClientRect(hwnd, &windowRect);
        int windowCenterX = windowRect.right / 2;
        int windowCenterY = windowRect.bottom / 2;

        // Static variable to keep track of the previous position of the mouse
        static POINT lastMousePos = cursorPos;

        // Skip processing if the cursor has just been set to the center
        static bool cursorMoved = false;

        // Calculate the change in mouse position (delta)
        int deltaX = cursorPos.x - lastMousePos.x;
        int deltaY = cursorPos.y - lastMousePos.y;

        // Only update yaw and pitch if the cursor has moved
        if (cursorMoved) {
            yaw -= deltaX * sensitivity;
            pitch -= deltaY * sensitivity;

            // Constrain pitch to avoid flipping the camera
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        // Check if the cursor hits the edge of the window
        if (cursorPos.x <= 0 || cursorPos.x >= windowRect.right - 1 || cursorPos.y <= 0 || cursorPos.y >= windowRect.bottom - 1) {
            // Move the cursor back to the center of the window if it's near the edge
            SetCursorPos(windowCenterX, windowCenterY);
            cursorMoved = false;  // Don't update the camera if the cursor was reset
        }
        else {
            cursorMoved = true;  // Allow camera update if the cursor is not reset
        }

        // Update the last position for the next frame
        lastMousePos = cursorPos;
    }


    // Move camera forward/backward
    void MoveForward(float amount) {
        DirectX::XMFLOAT3 forward;
        forward.x = cosf(DirectX::XMConvertToRadians(yaw));
        forward.y = 0.0f; // Maintain height
        forward.z = sinf(DirectX::XMConvertToRadians(yaw));

        DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR dir = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&forward), amount);

        // Update position
        DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(pos, dir));
        position.y = CAMERAHEIGHT; // Keep height constant
    }

    // Move camera left/right
    void Strafe(float amount) {
        DirectX::XMFLOAT3 right;
        right.x = sinf(DirectX::XMConvertToRadians(yaw));
        right.y = 0.0f; // Maintain height
        right.z = -cosf(DirectX::XMConvertToRadians(yaw));

        DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR dir = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&right), amount);

        // Update position
        DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(pos, dir));
        position.y = CAMERAHEIGHT; // Keep height constant
    }
    void LockMouseToWindow(HWND hwnd) {
        RECT windowRect;
        RECT rect;
        GetClientRect(hwnd, &rect);  // Get the client area of the window
        POINT topLeft = { rect.left, rect.top };
        ClientToScreen(hwnd, &topLeft);  // Convert client coordinates to screen coordinates
        windowRect.left = topLeft.x;
        windowRect.top = topLeft.y;
        windowRect.right = windowRect.left + rect.right;
        windowRect.bottom = windowRect.top + rect.bottom;

        ClipCursor(&windowRect);  // Restrict the cursor to the window's client area
        ShowCursor(FALSE);
    }
    void CenterMouse(HWND hwnd) {
        RECT rect;
        GetClientRect(hwnd, &rect);
        POINT center = { (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 };
        SetCursorPos(center.x, center.y);  // Reset mouse position to the center of the window
    }
};

