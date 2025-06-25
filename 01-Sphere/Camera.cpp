#include "Camera.h"
#include <math.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/quaternion.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 rotation, float fov, float aspect, float nearClip, float farClip,
               float topClip, float bottomClip, float leftClip, float rightClip, float camMoveSpeed, float camRotSpeed)
{
    Position = position;
    Rotation = rotation;
    FOV = fov;
    Aspect = aspect;
    NearClip = nearClip;
    FarClip = farClip;
    TopClip = topClip;
    BottomClip = bottomClip;
    LeftClip = leftClip;
    RightClip = rightClip;
    CamMoveSpeed = camMoveSpeed;
    CamRotSpeed = camRotSpeed;

    UpdateDirections();
}

glm::mat4 Camera::GetViewMatrix()
{

    glm::mat4 ViewMatrix(1.0f);

    ViewMatrix[0] = glm::vec4(Right, -dot(Right, Position));
    ViewMatrix[1] = glm::vec4(Up, -dot(Up, Position));
    ViewMatrix[2] = glm::vec4(-Forward, dot(Forward, Position));

    return glm::transpose(ViewMatrix);
}

glm::mat4 Camera::GetProjectionMatrix()
{

    glm::mat4 projection(1.0f);

    if (Perspective)
    {
        float height = tan(glm::radians(FOV / 2.0f));
        float width = height * Aspect;

        float Sx = 1 / width;
        float Sy = 1 / height;
        float Sz = -(FarClip + NearClip) / (FarClip - NearClip);
        float S0 = -2 * FarClip * NearClip / (FarClip - NearClip);

        projection[0][0] = Sx;
        projection[1][1] = Sy;
        projection[2][2] = Sz;
        projection[2][3] = -1;
        projection[3][2] = S0;
        projection[3][3] = 0.0f;
    }
    else
    {
        projection[0][0] = 2.0f / (RightClip - LeftClip);
        projection[3][0] = -(RightClip + LeftClip) / (RightClip - LeftClip);
        projection[1][1] = 2.0f / (TopClip - BottomClip);
        projection[2][2] = -2.0f / (FarClip - NearClip);
        projection[3][1] = -(TopClip + BottomClip) / (TopClip - BottomClip);
        projection[3][2] = -(FarClip + NearClip) / (FarClip - NearClip);

        if (Aspect > 1)
        {
            projection[0][0] = 2.0f / ((RightClip - LeftClip) * Aspect);
        }
        else if (Aspect < 1)
        {
            projection[1][1] = 2.0f * Aspect / (TopClip - BottomClip);
        }
    }

    return projection;
}

void Camera::Move(float x, float y, float z)
{
    Position += Forward * x + Up * y + Right * z;
}

void Camera::Move(glm::vec3 direction)
{

    Position += Forward * direction.x + Up * direction.y + Right * direction.z;
}

void Camera::Rotate(float pitch, float yaw, float roll)
{
    Rotation.z += roll;
    if (Rotation.z > 90)
        Rotation.z = 90;
    if (Rotation.z < -90)
        Rotation.z = -90;

    Rotation.x += pitch * glm::cos(glm::radians(Rotation.z)) + yaw * glm::sin(glm::radians(Rotation.z));
    Rotation.y += pitch * glm::sin(glm::radians(Rotation.z)) + yaw * glm::cos(glm::radians(Rotation.z));

    if (Rotation.x > 90)
        Rotation.x = 90;
    if (Rotation.x < -90)
        Rotation.x = -90;

    if (Rotation.y > 360)
        Rotation.y -= 360;
    if (Rotation.y < -360)
        Rotation.y += 360;

    if (Rotation.z > 360)
        Rotation.z -= 360;
    if (Rotation.z < -360)
        Rotation.z += 360;

    if (pitch != 0 || yaw != 0 || roll != 0)
        UpdateDirections();
}

void Camera::Rotate(glm::vec3 direction)
{
    Rotation += direction;

    if (Rotation.x > 90 || Rotation.x < -90)
        Rotation.x -= direction.x;

    if (Rotation.y > 360)
        Rotation.y -= 360;
    if (Rotation.y < -360)
        Rotation.y += 360;

    if (Rotation.z > 360)
        Rotation.z -= 360;
    if (Rotation.z < -360)
        Rotation.z += 360;

    if (direction != glm::vec3(0, 0, 0))
        UpdateDirections();
}

void Camera::UpdateDirections()
{
    Forward.x = sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
    Forward.y = sin(glm::radians(Rotation.x));
    Forward.z = cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
    Forward = glm::normalize(Forward);

    Right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), -Forward));

    glm::mat4 roll = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), Forward);
    Right = glm::mat3(roll) * Right;

    Up = glm::normalize(glm::cross(-Forward, Right));
}

bool perspective_pressed[] = {false, false};
void Camera::ProcessInputs(GLFWwindow *window, float DeltaTime)
{

    glm::vec3 posChange(0, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        posChange.x += CamMoveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        posChange.x -= CamMoveSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        posChange.z += CamMoveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        posChange.z -= CamMoveSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        posChange.y += CamMoveSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        posChange.y -= CamMoveSpeed;

    glm::vec3 rotChange(0, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
        rotChange.x += CamRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
        rotChange.x -= CamRotSpeed;

    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
        rotChange.y += CamRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
        rotChange.y -= CamRotSpeed;

    if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
        rotChange.z -= CamRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
        rotChange.z += CamRotSpeed;

    perspective_pressed[0] = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    if (perspective_pressed[0] && perspective_pressed[0] != perspective_pressed[1])
        Perspective = !Perspective;
    perspective_pressed[1] = perspective_pressed[0];

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        Position = glm::vec3(0, 0, 0);
        Rotation = glm::vec3(0, 0, 0);
        UpdateDirections();
        return;
    }

    Move(posChange * DeltaTime);
    Rotate(rotChange * DeltaTime);
}
