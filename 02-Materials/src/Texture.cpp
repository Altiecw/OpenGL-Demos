#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__ 1
#include <STB/stb_image_write.h>
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

void Screenshot(const int width, const int height, const char* name) {

	GLubyte* data = new GLubyte[width * height * 4];

	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_flip_vertically_on_write(true);

	stbi_write_png(name, width, height, 4, data, width * 4);

	delete[] data;
}