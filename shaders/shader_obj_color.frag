#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float     shininess;
};

uniform Material material;

struct Light {
  vec3 position;
  /*vec3 direction;*/

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform Light light;

void main()
{
  // Ambient Light Component
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

  // Diffuse Light Component
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  /*vec3 lightDir = normalize(-light.direction);*/
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

  // Specular Light Component
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

  // Distance Attenuation
  float distanceFromLight = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distanceFromLight +
                      light.quadratic * (distanceFromLight * distanceFromLight));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  // Final Color
  vec3 result = ambient + diffuse + specular;

  FragColor = vec4(result, 1.0);
}
