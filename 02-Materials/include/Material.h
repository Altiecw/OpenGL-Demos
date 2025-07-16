#ifndef MATERIAL_H
#define MATERIAL_H

#include <GLM/glm.hpp>

const unsigned int MaxMaterials = 100;

struct Material {
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    int DiffuseTexture;
    glm::vec3 Specular;
    float Shininess;
};

Material Materials[MaxMaterials];

#endif
