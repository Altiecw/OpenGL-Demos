#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
	unsigned int id = 0;
	int width, height, channels, unit = 0;
	Texture(const char* source);
	void Bind(int slot = 0);
	void Unbind();
};

void Screenshot(const int width, const int height, const char* name);

#endif
