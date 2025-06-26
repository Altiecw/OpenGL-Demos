#include "BufferObjects.h"

void BO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void BO::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BO::Delete()
{
    glDeleteBuffers(1, &id);
}

BO::~BO()
{
    this->Delete();
}

VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

void VAO::SetAttribute(VBO* vbo, GLuint layout, GLuint components, GLenum type, GLsizei stride, void *offset)
{
    vbo->Bind();
    glVertexAttribPointer(layout, components, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    vbo->Unbind();
}

void VAO::Bind()
{
    glBindVertexArray(id);
}

void VAO::Unbind()
{
    glBindVertexArray(0);
}

void VAO::Delete()
{
    glDeleteVertexArrays(1, &id);
}

VBO::VBO(GLfloat *vertices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

EBO::EBO(GLuint *indices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
