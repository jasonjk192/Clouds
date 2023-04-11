// Credits: https://learnopengl.com/

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;
const int WIDTH = 1280;
const int HEIGHT = 720;
const float NEAR = 0.1f;
const float FAR = 100.f;
const float ASPECTRATIO = float(WIDTH) / float(HEIGHT);

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float fov;
    float aspectRatio;
    float width;
    float height;
    float near;
    float far;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    bool enableCameraMovement;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)),
        MovementSpeed(SPEED),
        MouseSensitivity(SENSITIVITY),
        enableCameraMovement(false),
        Position(position),
        WorldUp(up),
        Yaw(yaw),
        Pitch(pitch),
        aspectRatio(ASPECTRATIO),
        fov(FOV),
        width(WIDTH),
        height(HEIGHT),
        near(NEAR),
        far(FAR)
    {
        updateCameraVectors();
    }

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    {
        Camera(glm::vec3(posX, posY, posZ) , glm::vec3(upX, upY, upZ) , yaw, pitch);
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::vec2 GetScreenSize()
    {
        return glm::vec2(width, height);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == CameraMovement::FORWARD)
            Position += Front * velocity;
        if (direction == CameraMovement::BACKWARD)
            Position -= Front * velocity;
        if (direction == CameraMovement::LEFT)
            Position += Right * velocity;
        if (direction == CameraMovement::RIGHT)
            Position -= Right * velocity;
        if (direction == CameraMovement::UP)
            Position += Up * velocity;
        if (direction == CameraMovement::DOWN)
            Position -= Up * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }

    void SaveCameraSettings()
    {
        std::ofstream camsettings;
        camsettings.open("camera.txt");
        camsettings << Position.x << "\n";
        camsettings << Position.y << "\n";
        camsettings << Position.z << "\n";
        camsettings << Pitch << "\n";
        camsettings << Yaw << "\n";
        camsettings << fov << "\n";
        camsettings << Front.x << "\n";
        camsettings << Front.y << "\n";
        camsettings << Front.z << "\n";
        camsettings << Up.x << "\n";
        camsettings << Up.y << "\n";
        camsettings << Up.z << "\n";
        camsettings << Right.x << "\n";
        camsettings << Right.y << "\n";
        camsettings << Right.z << "\n";
        camsettings.close();
    }

    void LoadCameraSettings()
    {
        std::string line;
        std::ifstream camsettings("camera.txt");
        float values[15];
        if (camsettings.is_open())
        {
            int i = 0;
            while (std::getline(camsettings, line))
            {
                values[i] = std::stof(line);
                i++;
            }
            camsettings.close();
            Position = glm::vec3(values[0], values[1], values[2]);
            Pitch = values[3];
            Yaw = values[4];
            fov = values[5];
            Front = glm::vec3(values[6], values[7], values[8]);
            Up = glm::vec3(values[9], values[10], values[11]);
            Right = glm::vec3(values[12], values[13], values[14]);
        }
        else std::cerr << "Unable to open file";
    }

    void Reset()
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw = YAW;
        Pitch = PITCH;
        updateCameraVectors();
    }

    glm::mat4x4 GetFrustumCorners()
    {
        glm::vec4 fCorners[4];
        CalculateFrustumCorners(fCorners);
        glm::mat4x4 frustumCorners = glm::mat4x4(fCorners[0], fCorners[1], fCorners[2], fCorners[3]);
        return frustumCorners;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }

    void CalculateFrustumCorners(glm::vec4 corners[4])
    {
        float near = 0.1f;
        float far = 100.f;

        glm::vec3 farCenter = Position - Front * far;
        float farHeight = 2 * tan(fov / 2) * far;
        float farWidth = farHeight * aspectRatio;

        corners[0] = glm::vec4(farCenter + Up * (farHeight * 0.5f) - Right * (farWidth * 0.5f), 1); //farTopLeft
        corners[1] = glm::vec4(farCenter + Up * (farHeight * 0.5f) + Right * (farWidth * 0.5f), 1);    //farTopRight
        corners[2] = glm::vec4(farCenter - Up * (farHeight * 0.5f) - Right * (farWidth * 0.5f), 1);  //farBottomLeft
        corners[3] = glm::vec4(farCenter - Up * (farHeight * 0.5f) + Right * (farWidth * 0.5f), 1); //farBottomRight
    }

};
#endif