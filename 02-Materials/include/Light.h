#ifndef LIGHT_H
#define LIGHT_H

#include <GLM/glm.hpp>

struct Light
{
	glm::vec3 Colour;
};

struct PointLight : Light 
{
	glm::vec3 Position;
	float attenuation;
};

struct DirectionalLight : Light
{
	glm::vec3 Direction;
};

struct SpotLight : Light
{
	glm::vec3 Direction;
};

#endif
