#version 330 core
#define NR_POINT_LIGHTS 4

#include material.frag
#include lights.frag

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform vec3 viewPos;

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform DirLight dirLight;
uniform Material material;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D shadowMap;
uniform samplerCube shadowCube;
uniform float far_plane;

#include phong.frag

void main() {
  vec3 normal  = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 result  = vec3(0, 0, 0);

  //result += CalcDirLight(dirLight, normal, viewDir);

  //for(int i = 0; i < NR_POINT_LIGHTS; i++) {
    //result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
  //}

  //result += CalcSpotLight(spotLight, normal, FragPos, viewDir);

  result += CalcPointLight(pointLights[2], normal, FragPos, viewDir);

  result *= material.color;

  //result = CalcPointLightShadowDebug(FragPos, pointLights[2].position);

  FragColor = vec4(result, 1.0);
}
