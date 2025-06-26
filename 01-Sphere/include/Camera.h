#ifndef CLASS_CAMERA_H
#define CLASS_CAMERA_H

#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>

class Camera
{
  private:
    // Update the Forward, up, and right vector
    void UpdateDirections();

  public:
    // The position of the camera
    glm::vec3 Position = glm::vec3(0, 0, 0);
    // The pitch, yaw, and roll of the camera
    glm::vec3 Rotation = glm::vec3(0, 0, 0);
    // The camera's field of view in degrees. In perspective rendering, this determines how much the camera can see at a
    // given distance
    float FOV;
    // The Aspect Ratio is how many times the display of the camera is wider than it is longer. If one, then the display
    // is a square.
    float Aspect;
    // The closest an object can be from the camera before not being rendered
    float NearClip;
    // The furthest an object can be from the camera before not being rendered
    float FarClip;
    // The highest an object can be from the camera before not being rendered; Used in Orthographic matrices only
    float TopClip;
    // The lowest an object can be from the camera before not being rendered; Used in Orthographic matrices only
    float BottomClip;
    // The leftmost an object can be from the camera before not being rendered; Used in Orthographic matrices only
    float LeftClip;
    // The rightmost an object can be from the camera before not being rendered; Used in Orthographic matrices only
    float RightClip;
    // How many units can the camera move in any direction per second
    float CamMoveSpeed;
    // How many degrees can the camera turn in any direction per second
    float CamRotSpeed;
    // How many degrees can the camera turn in any direction per second
    int ViewPortWidth;
    // How many degrees can the camera turn in any direction per second
    int ViewportHeight;
    // Is this camera using perspective? Uses orthographic projection is false.
    bool Perspective = true;

    // The relative forward vector for the camera
    glm::vec3 Forward = glm::vec3(0, 0, 0);
    // The relative up direction for the camera
    glm::vec3 Up = glm::vec3(0, 0, 0);
    // The relative right direction for the camera
    glm::vec3 Right = glm::vec3(0, 0, 0);

    Camera(glm::vec3 position, glm::vec3 rotation, float fov = 0, float aspect = 0, float nearClip = 0,
           float farClip = 0, float topClip = 0, float bottomClip = 0, float leftClip = 0, float rightClip = 0,
           float camMoveSpeed = 0, float camRotSpeed = 0);

    /*
    Creates a matrix that transforms position coordinates in world space to be in view space(relative to the camera's
    position and rotation). To apply this, multiply a vec4 for positions (the 4th value being 1) by this matrix
    */
    glm::mat4 GetViewMatrix();
    /*
    Create a matrix that transforms position coordinates in view space to clip space(relative to the bounds of a box
    where only objects insides will be rendered). To apply this, multiply a vec4 for positions (the 4th value being a 1)
    by a view matrix and then by this matrix
    */
    glm::mat4 GetProjectionMatrix();

    // Process inputs for movement, rotation, and other things
    void ProcessInputs(GLFWwindow *window, float DeltaTime);

    // Move the camera by the given x, y, z direction
    void Move(float x, float y, float z);
    // Move the camera by the given x, y, z direction
    void Move(glm::vec3 direction);

    // Rotate the camera by the given pitch, yaw, and roll
    void Rotate(float pitch, float yaw, float roll);
    // Rotate the camera by the given pitch, yaw, and roll
    void Rotate(glm::vec3 direction);
};

#endif
