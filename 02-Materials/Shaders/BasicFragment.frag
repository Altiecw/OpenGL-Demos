#version 430 core
out vec4 Colour;

in vec2 TexCoord;

uniform sampler2D colourTexture;

void main(){
    Colour = texture(colourTexture, TexCoord);
}