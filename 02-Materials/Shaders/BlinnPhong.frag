#version 430 core

struct Material {
    vec3 Ambient;
    vec3 Diffuse;
    int DiffuseTexture;
    vec3 Specular;
    float Shininess;
};

out vec4 Colour;

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

const int MAX_MATERIALS = 100;
const int MAX_TEXTURES = 32;

uniform float ambience;
uniform sampler2D DiffuseTexture;
uniform vec3 lightColour;
uniform vec3 lightPos;
uniform sampler2D Textures[MAX_TEXTURES];
uniform Material Materials[MAX_MATERIALS];
uniform int MatIndex;
uniform vec3 camPos;
uniform float Shininess;

vec3 ambient, diffuse, specular;

void main(){
    
    vec3 BaseColour =  vec3(texture(Textures[Materials[MatIndex].DiffuseTexture], UV));

    ambient = lightColour * Materials[MatIndex].Ambient * ambience;

    vec3 Norm = normalize(Normal);
    vec3 LightDir = normalize(lightPos - FragPos);
    diffuse = lightColour *  max(dot(Norm, LightDir), 0);

    vec3 reflectDir = normalize(reflect(-LightDir, Norm));
    vec3 camDir = normalize(camPos - FragPos);
    float spec;
    //spec = pow(max(dot(reflectDir, camDir), 0), Materials[MatIndex].Shininess); // Phong

    vec3 HalfVector = normalize(camDir + LightDir);

    spec = pow(max(dot(Norm, HalfVector), 0),  Materials[MatIndex].Shininess); // Blinn-Phong

    specular = lightColour * spec * Materials[MatIndex].Specular;
    

    //Colour = vec4((ambience + diffuse + specular), 1.0);
    Colour = vec4((ambient + diffuse) * BaseColour + specular, 1.0);
}