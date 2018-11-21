#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "../config.h"
#include "../utils/tools.h"
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROLL_LEFT,
    ROLL_RIGHT
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = default_position, glm::vec3 up = default_up, float yaw = YAW, float pitch = PITCH) : Front(default_front), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(default_front), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == ROLL_LEFT)
            Yaw -= YAW_SPEED;
        if (direction == ROLL_RIGHT)
            Yaw += YAW_SPEED;
        updateCameraVectors();
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > MAX_PITCH)
                Pitch = MAX_PITCH;
            if (Pitch < MIN_PITCH)
                Pitch = MIN_PITCH;
        }
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        if (Zoom >= MIN_ZOOM && Zoom <= MAX_ZOOM)
            Zoom -= yoffset;
        if (Zoom <= MIN_ZOOM)
            Zoom = MIN_ZOOM;
        if (Zoom >= MAX_ZOOM)
            Zoom = MAX_ZOOM;
    }

private:

    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));

//        static int debugflag = 1;
//        if(debugflag){
//            std::cout << "Front: ";
//            Tools::PrintVec3(Front);
//            std::cout << "Right: ";
//            Tools::PrintVec3(Right);
//            std::cout << "Up: ";
//            Tools::PrintVec3(Up);
//            std::cout << "WorldUp: ";
//            Tools::PrintVec3(WorldUp);
//        }
//        debugflag = 0;
    }
};
#endif