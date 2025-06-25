#version 430 core
out vec4 Colour;

in vec3 newColour;

void main(){
    Colour = vec4(abs(newColour), 1.0f);
}