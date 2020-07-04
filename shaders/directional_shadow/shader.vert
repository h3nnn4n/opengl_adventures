#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 shadow_projection;
uniform mat4 shadow_view;
uniform mat4 model;

void main()
{
  gl_Position = shadow_projection * shadow_view * model * vec4(aPos, 1.0);
}
