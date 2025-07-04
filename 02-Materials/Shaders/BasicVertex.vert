#version 430 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(Pos, 1.0f);
	Normal = normal;
	TexCoord = vec2(texCoord.x, texCoord.y);
}