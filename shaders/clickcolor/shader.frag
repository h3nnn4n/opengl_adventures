#version 330 core
in vec3 Normal;

out vec4 FragColor;

uniform vec3 color_id;

// zero is for colors
// one is for normals
uniform float mode;

void main() {
  vec4 color = vec4(color_id, 1.0);
  vec4 normal = vec4(Normal, 1.0);

  FragColor = mode * normal + (1.0f - mode) * color;
}
