#include "BufferObjects.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shader.h"
#include "Shapes.h"
#include "Texture.h"

#include <chrono>
#include <filesystem>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sys/stat.h>

const int initWidth = 640;
const int initHeight = 640;
const char *initName = "Material Demo";

bool isFullscreen = false;
int displayMonitor = 1;
int monitorCount;
int windowPosition[] = {0, 0};
int windowSize[] = {0, 0};
const GLFWvidmode *monitorVMode;
GLFWmonitor **monitors;

float DeltaTime = 0;
float LastTime = 0;
Camera *camera;

std::string ScreenshotDirectory;
std::string RecordingDirectory;
bool isRecording = false;
int recordedImages = 0;

std::vector<std::pair<std::string, std::string>> ShaderSources;
std::vector<Shader> Shaders;
int selectedShader = 0;

const char* colourTextureSource = "Textures/Earth8kDaymap.jpg";

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
        glViewport(0, 0, monitorVMode->width, monitorVMode->height);
    }
    else {
        glViewport(0, 0, windowSize[0], windowSize[1]);
    }

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Failure to init GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    std::cout << "Setting up shaders" << std::endl;

    ShaderSources.push_back(std::pair<std::string, std::string>("Shaders/BlinnPhong.vert", "Shaders/BlinnPhong.frag"));
    ShaderSources.push_back(std::pair<std::string, std::string>("Shaders/PositionColour.vert", "Shaders/PositionColour.frag"));
    
    for (int i = 0; i < ShaderSources.size(); ++i) 
    {
        Shader newShader;
        if (!newShader.Init(ShaderSources[i].first.c_str(), ShaderSources[i].second.c_str()))
        {
            std::cout << "Failed to setup shaders for" << ShaderSources[i].first << " and " << ShaderSources[i].second << std::endl;
            return -1;
        }
        Shaders.push_back(newShader);
    }

    std::cout << "Setting up camera" << std::endl;
    camera = new Camera(glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), 45.0f, (float)windowSize[0] / (float)windowSize[1],
                        0.001f, 25.0f, 1, -1, -1, 1, 0.5f, 30.0f);

    if (isFullscreen)
    {
        camera->Aspect = (float)monitorVMode->width / (float)monitorVMode->height;
    }

    std::cout << "Loading images" << std::endl;

    Texture colourTexture(colourTextureSource);

    std::cout << "Creating shapes" << std::endl;
    Sphere sphere;
    sphere.GenerateLatLongSphere_PNU(64, 1.0f);

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    float sphereRotateSpeed = 10.0f;

    PointLight light;
    light.Position = glm::vec3(3.0f, 0, 0);
    light.Colour = glm::vec3(1.0f, 1.0f, 1.0f);

    std::cout << "Setting up Materials" << std::endl;

    Material Earth;

    Earth.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    Earth.Diffuse = glm::vec3(1, 1, 1);
    Earth.Specular = glm::vec3(1, 1, 1);
    Earth.DiffuseTexture = 0;
    Earth.Shininess = 32;

    Materials[0] = Earth;

    std::cout << "Setting up buffers" << std::endl;

    VAO SphereVAO;

    SphereVAO.Bind();
    VBO SphereVBO(&sphere.vertices[0], sphere.vertices.size() * sizeof(float));
    EBO SphereEBO(&sphere.indices[0], sphere.indices.size() * sizeof(int));
    SphereVAO.SetAttribute(&SphereVBO, 0, 3, GL_FLOAT, sizeof(GL_FLOAT) * 8, (void *)0);
    SphereVAO.SetAttribute(&SphereVBO, 1, 3, GL_FLOAT, sizeof(GL_FLOAT) * 8, (void *)(sizeof(GL_FLOAT) * 3));
    SphereVAO.SetAttribute(&SphereVBO, 2, 2, GL_FLOAT, sizeof(GL_FLOAT) * 8, (void*)(sizeof(GL_FLOAT) * 6));

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

        Shaders[selectedShader].Activate();

        model = glm::rotate(model, glm::radians(sphereRotateSpeed * DeltaTime), glm::vec3(0, 1, 0));
        Shaders[selectedShader].SetMat4("model", model);
        Shaders[selectedShader].SetMat3("modelNormal", glm::mat3(glm::transpose(glm::inverse(model))));
        Shaders[selectedShader].SetMat4("view", camera->GetViewMatrix());
        Shaders[selectedShader].SetMat4("projection", camera->GetProjectionMatrix());
        Shaders[selectedShader].SetVec3("camPos", camera->Position);

        Shaders[selectedShader].SetVec3("Materials[0].Ambient", Materials[0].Ambient);
        Shaders[selectedShader].SetVec3("Materials[0].Diffuse", Materials[0].Diffuse);
        Shaders[selectedShader].SetInt("Materials[0].DiffuseTexture", colourTexture.unit);
        Shaders[selectedShader].SetVec3("Materials[0].Specular", Materials[0].Specular);
        Shaders[selectedShader].SetFloat("Materials[0].Shininess", Materials[0].Shininess);
        Shaders[selectedShader].SetInt("MatIndex", 0);
        Shaders[selectedShader].SetFloat("ambience", 0.1f);
        Shaders[selectedShader].SetVec3("lightColour", light.Colour);
        Shaders[selectedShader].SetVec3("lightPos", light.Position);
       

        colourTexture.Bind();
        SphereVAO.Bind();
        glDrawElements(GL_TRIANGLES, sphere.indices.size() * sizeof(int), GL_UNSIGNED_INT, 0);
        SphereVAO.Unbind();

        if (isRecording) {
            Screenshot(windowSize[0], windowSize[1],(RecordingDirectory + std::to_string(recordedImages) + ".png").c_str());
            ++recordedImages;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    SphereVAO.Delete();
    for (Shader shader : Shaders) {
        shader.Delete();
    }
    glfwTerminate();
    return 0;
}

bool fullscreen_pressed[] = {false, false};
bool changeshaders_pressed[] = { false, false };
bool recording_pressed[] = {false, false};
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

    changeshaders_pressed[0] = glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS;
    if (changeshaders_pressed[0] && changeshaders_pressed[0] != changeshaders_pressed[1]) {
        if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
        {
            selectedShader = (selectedShader - 1) % Shaders.size();
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
        {
            selectedShader = (selectedShader + 1) % Shaders.size();
        }
    }
    changeshaders_pressed[1] = changeshaders_pressed[0];

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        Screenshot(windowSize[0], windowSize[1], "Screenshot/output.png");
    }

    recording_pressed[0] = glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS;
    if (recording_pressed[0] && recording_pressed[0] != recording_pressed[1])
    {
        isRecording = !isRecording;

        if (isRecording) {
            
            struct tm timenow;

            time_t now = time(NULL);
            gmtime_s(&timenow, &now);
            char date[26];
            strftime(date, sizeof(date), "%Y-%m-%d_%H-%M-%S", &timenow);

            std::string directory("Screenshot/");
            directory.append(date);

            if (!std::filesystem::exists(directory)) std::filesystem::create_directory(directory);

            RecordingDirectory = directory + "/";
            recordedImages = 0;
        }
    }
    recording_pressed[1] = recording_pressed[0];

    camera->ProcessInputs(window, DeltaTime);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (camera != NULL)
        camera->Aspect = (float)width / (float)height;
}
