#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <GL/glew.h>

#include "Texture.h"

Texture::Texture(const char* source) 
{

	unsigned char* data = stbi_load(source, &width, &height, &channels, 0);

	if (!data)
	{
		free(data);
		return;
	}

	glGenTextures(1, &id);
	
	this->Bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_set_flip_vertically_on_load(true);

	glGenerateMipmap(GL_TEXTURE_2D);

	free(data);
}

void Texture::Bind(int slot) 
{
	if (0 <= unit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
	{
		unit = slot;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
}

void Texture::Unbind() 
{
	glBindTexture(GL_TEXTURE_2D, 0);
}