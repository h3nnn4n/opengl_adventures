#version 330 core

out vec4 FragColor;

uniform vec3 color_id;

void main() {
  FragColor = vec4(color_id, 1.0);
}
