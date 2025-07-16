#include "Shader.h"
#include <fstream>
#include <iostream>
#include <string>

bool Shader::InitInternal(const char *vertexShader, const char *fragmentShader)
{

    unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(VertexShader, 1, &vertexShader, NULL);
    glCompileShader(VertexShader);

    int result;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &result);
    if (result == 0)
    {
        char ErrorResults[512];
        glGetShaderInfoLog(VertexShader, 512, NULL, ErrorResults);
        std::cout << "Error in vertex shader compilation: " << ErrorResults << std::endl;
        return false;
    }

    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &fragmentShader, NULL);
    glCompileShader(FragmentShader);
    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &result);
    if (result == 0)
    {
        char ErrorResults[512];
        glGetShaderInfoLog(FragmentShader, 512, NULL, ErrorResults);
        std::cout << "Error in fragment shader compilation: " << ErrorResults << std::endl;
        return false;
    }

    id = glCreateProgram();
    glAttachShader(id, VertexShader);
    glAttachShader(id, FragmentShader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if (result == 0)
    {
        char ErrorResults[512];
        glGetProgramInfoLog(id, 512, NULL, ErrorResults);
        std::cout << "Error in shader program linking: " << ErrorResults << std::endl;
        return false;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    return true;
}

bool Shader::Init(const char *vertexSource, const char *fragmentSource)
{

    std::ifstream vs(vertexSource, std::ios::binary);
    if (!vs)
    {
        std::cout << "Unable to open Vertex Shader file at " << vertexSource << std::endl;
        return false;
    }

    std::string vsText;
    vsText.assign(std::istreambuf_iterator<char>(vs), std::istreambuf_iterator<char>());
    vs.close();

    if (!vs)
    {
        std::cout << "Unable to read all " << vertexSource << ", only " << vs.gcount() << std::endl;
        return false;
    }

    std::ifstream fs(fragmentSource, std::ios::binary);
    if (!fs)
    {
        std::cout << "Unable to open Fragment Shader file at " << fragmentSource << std::endl;
        return false;
    }

    std::string fsText;
    fsText.assign(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>());
    fs.close();

    if (!fs)
    {
        std::cout << "Unable to read all " << fragmentSource << ", only " << fs.gcount() << std::endl;
        return false;
    }

    return InitInternal(vsText.c_str(), fsText.c_str());
}

void Shader::Activate()
{
    glUseProgram(id);
}

void Shader::Deactivate()
{
    glUseProgram(0);
}

void Shader::Delete()
{
    glDeleteProgram(id);
}

void Shader::SetBool(std::string name, bool value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetFloat(std::string name, float value)
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetInt(std::string name, int value)
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetMat3(std::string name, glm::mat3 value)
{
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, false, &value[0][0]);
}

void Shader::SetMat4(std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, false, &value[0][0]);
}

void Shader::SetVec3(std::string name, glm::vec3 value)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec3(std::string name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
