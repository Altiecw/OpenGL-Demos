#ifndef BUFFEROBJECT_H
#define BUFFEROBJECT_H

#include <GL/glew.h>

class BO 
{
protected:
    GLuint id;
public:
    virtual ~BO();
    virtual void Bind();
    virtual void Unbind();
    virtual void Delete();
};

/*  
    A Vertex Buffer Object (VBO) is an OpenGL object which holds a list of vertices and then binds them to OpenGL so
    they'll be drawn when a command is ran to draw what is in the buffer such as glDrawArrays() or glDrawElements() 
*/
class VBO : public BO
{
  public:
    // Creates a Vertex Buffer Object (VBO) for the given vertices
    VBO(GLfloat *vertices, GLsizeiptr size);
};

/*
    An Element Buffer Object (EBO) is an OpenGL object which holds a list of indices. Used with a VAO and VBO, it tells
    OpenGL the order of vertices in the buffer to draw
 */
class EBO : public BO
{
  public:
    // Creates an Element Buffer Object (EBO) for the given indices
    EBO(GLuint *indices, GLsizeiptr size);
    virtual void Bind() override;
    virtual void Unbind() override;
};

/*
    A Vertex Array Object(VBO) is an OpenGL object which holds buffers of vertices, indices for the buffers, and the
    specifics of what the values in the buffer mean. Binding a VAO makes OpenGL draw the vertices in its buffere when a
    command is ran to draw what is in the buffer such as glDrawArrays() or glDrawElements()
*/
class VAO
{
  private:
    GLuint id;

  public:
    // Creates a Vertex Array Object (VBO) for the given vertices
    VAO();
    void SetAttribute(VBO* vbo, GLuint layout, GLuint components, GLenum type, GLsizei stride, void *offset);
    void Bind();
    void Unbind();
    void Delete();
};

#endif
