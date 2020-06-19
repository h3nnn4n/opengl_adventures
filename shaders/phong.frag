float ShadowCalculation(vec4 fragPosLightSpace) {
  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.z > 1.0) return 0.0;

  float closestDepth = texture(shadowMap, projCoords.xy).r;

  float currentDepth = projCoords.z;

  float bias = 0.0025f;
  float shadow = 0.0;
  vec2 texelSize = 0.75f / textureSize(shadowMap, 0);

  int sample_range = 3;
  float sample_count = 0.0f;

  for (int x = -sample_range; x <= sample_range; ++x) {
    for (int y = -sample_range; y <= sample_range; ++y) {
      float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;

      sample_count += 1;
    }
  }

  shadow /= sample_count;

  return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  if (light.active == 0) return vec3(0, 0, 0);

  vec3 lightDir = normalize(-light.direction);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

  // combine results
  vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse, TexCoords));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));

  float shadow = ShadowCalculation(FragPosLightSpace);

  return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  if (light.active == 0) return vec3(0, 0, 0);

  vec3 lightDir = normalize(light.position - fragPos);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

  // attenuation
  float distance    = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
                      light.quadratic * (distance * distance));

  // combine results
  vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse, TexCoords));
  vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));

  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;

  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  if (light.active == 0) return vec3(0, 0, 0);

  vec3 lightDir = normalize(light.position - FragPos);

  float theta     = dot(lightDir, normalize(-light.direction));
  float epsilon   = light.cutOff - light.outterCutOff;
  float intensity = clamp((theta - light.outterCutOff) / epsilon, 0.0, 1.0);

  // Ambient Light Component
  vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));

  // Diffuse Light Component
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));

  // Specular Light Component
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));

  // Distance Attenuation
  float distanceFromLight = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distanceFromLight +
                      light.quadratic * (distanceFromLight * distanceFromLight));

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  diffuse *= intensity;
  specular *= intensity;

  return (ambient + diffuse + specular);
}
