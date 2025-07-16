#version 430 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

uniform mat4 model;
uniform mat3 modelNormal;
uniform mat4 view;
uniform mat4 projection;

void main(){
	Normal = modelNormal * normal;
	UV = uv;
	FragPos = vec3(model * vec4(Pos, 1.0));

	gl_Position = projection * view * model * vec4(Pos, 1.0f);
}