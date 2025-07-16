#include "BufferObjects.h"
#include "Camera.h"
#include "Shader.h"
#include "Shapes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int initWidth = 640;
const int initHeight = 640;
const char *initName = "Sphere Demo";

bool isFullscreen = false;
int displayMonitor = 2;
int monitorCount;
int windowPosition[] = {0, 0};
int windowSize[] = {0, 0};
const GLFWvidmode *monitorVMode;
GLFWmonitor **monitors;

float DeltaTime = 0;
float LastTime = 0;
Camera *camera;

float BackgroundColour[] = {
    0.2f,
    0.0625f,
    0.2f,
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{

    std::cout << "Initialising" << std::endl;

    if (!glfwInit())
    {
        std::cout << "Failure to init GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    GLFWwindow *window;
    monitors = glfwGetMonitors(&monitorCount);
    window = glfwCreateWindow(initWidth, initHeight, initName, NULL, NULL);
    glfwGetWindowPos(window, &windowPosition[0], &windowPosition[1]);
    glfwGetWindowSize(window, &windowSize[0], &windowSize[1]);

    if (window == NULL)
    {
        std::cout << "Failure to create window" << std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (isFullscreen)
    {
        if (displayMonitor >= monitorCount)
            displayMonitor = 0;
        monitorVMode = glfwGetVideoMode(monitors[displayMonitor]);
        glfwSetWindowMonitor(window, monitors[displayMonitor], 0, 0, monitorVMode->width, monitorVMode->height,
                             monitorVMode->refreshRate);
    }

    if (isFullscreen)
    {
        glViewport(0, 0, monitorVMode->width, monitorVMode->height);
    }
    else
    {
        glViewport(0, 0, windowSize[0], windowSize[1]);
    }

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Failure to init GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    std::cout << "Setting up shaders" << std::endl;

    Shader BasicShader;
    if (!BasicShader.Init("Shaders/BasicVertex.vert", "Shaders/BasicFragment.frag"))
    {
        std::cout << "Failed to setup shaders" << std::endl;
        return -1;
    }

    std::cout << "Setting up camera" << std::endl;
    camera = new Camera(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), 45.0f, (float)windowSize[0] / (float)windowSize[1],
                        0.001f, 25.0f, 1, -1, -1, 1, 0.5f, 30.0f);

    if (isFullscreen)
    {
        camera->Aspect = (float)monitorVMode->width / (float)monitorVMode->height;
    }

    std::cout << "Creating shapes" << std::endl;
    Sphere sphere;
    sphere.GenerateUVSphere(64, 0.5f);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float sphereRotateSpeed = 10.0f;

    std::cout << "Setting up buffers" << std::endl;

    VAO SphereVAO;

    SphereVAO.Bind();
    VBO SphereVBO(&sphere.vertices[0], sphere.vertices.size() * sizeof(float));
    EBO SphereEBO(&sphere.indices[0], sphere.indices.size() * sizeof(int));
    SphereVAO.SetAttribute(&SphereVBO, 0, 3, GL_FLOAT, sizeof(GL_FLOAT) * 6, (void *)0);
    SphereVAO.SetAttribute(&SphereVBO, 1, 3, GL_FLOAT, sizeof(GL_FLOAT) * 6, (void *)(sizeof(GL_FLOAT) * 3));

    SphereVAO.Unbind();
    SphereVBO.Unbind();
    SphereEBO.Unbind();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(5);
    glPointSize(5);

    glEnable(GL_DEPTH_TEST);

    std::cout << "Starting Loop!" << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        float CurrentTime = glfwGetTime();
        DeltaTime = CurrentTime - LastTime;
        LastTime = CurrentTime;

        processInput(window);

        glClearColor(BackgroundColour[0], BackgroundColour[1], BackgroundColour[2], 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BasicShader.Activate();

        model = glm::rotate(model, glm::radians(sphereRotateSpeed * DeltaTime), glm::vec3(0, 1, 0));
        BasicShader.SetMat4("model", model);
        BasicShader.SetMat4("view", camera->GetViewMatrix());
        BasicShader.SetMat4("projection", camera->GetProjectionMatrix());

        SphereVAO.Bind();
        glDrawElements(GL_TRIANGLES, sphere.indices.size() * sizeof(int), GL_UNSIGNED_INT, 0);
        SphereVAO.Unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    SphereVAO.Delete();
    BasicShader.Delete();

    glfwTerminate();
    return 0;
}

bool fullscreen_pressed[] = {false, false};
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    fullscreen_pressed[0] = glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS;
    if (fullscreen_pressed[0] && fullscreen_pressed[0] != fullscreen_pressed[1])
    {
        isFullscreen = !isFullscreen;

        if (isFullscreen)
        {
            if (displayMonitor >= monitorCount)
                displayMonitor = 0;
            monitorVMode = glfwGetVideoMode(monitors[displayMonitor]);
            glfwGetWindowPos(window, &windowPosition[0], &windowPosition[1]);
            glfwGetWindowSize(window, &windowSize[0], &windowSize[1]);
            glfwSetWindowMonitor(window, monitors[displayMonitor], 0, 0, monitorVMode->width, monitorVMode->height,
                                 monitorVMode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(window, NULL, windowPosition[0], windowPosition[1], windowSize[0], windowSize[1], 0);
        }
    }
    fullscreen_pressed[1] = fullscreen_pressed[0];

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }

    camera->ProcessInputs(window, DeltaTime);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (camera != NULL)
        camera->Aspect = (float)width / (float)height;
}
