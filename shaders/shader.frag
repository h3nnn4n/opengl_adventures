#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float colorOverrideIntensity;
uniform float time;

void main()
{
  float pi = 3.141592653;

  vec4 override = vec4(
    colorOverrideIntensity * sin(time),
    colorOverrideIntensity * sin(time + pi / 3.0),
    colorOverrideIntensity * sin(time + pi / 3.0 * 2.0),
    1.0
  );

  FragColor = vec4(ourColor, 1.0) + override;
  FragColor+= texture(ourTexture, TexCoord);
}
