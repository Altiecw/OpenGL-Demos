#ifndef CLASS_SHADER_H
#define CLASS_SHADER_H

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <string>

class Shader
{
  private:
    GLuint id;
    bool InitInternal(const char *vertexShader, const char *fragmentShader);

  public:
    bool Init(const char *vertexSource, const char *fragmentSource);
    void Activate();
    void Deactivate();
    void Delete();

    void SetBool(std::string name, bool value);
    void SetInt(std::string name, int value);
    void SetFloat(std::string name, float value);
    void SetMat4(std::string name, glm::mat4);
};

#endif
