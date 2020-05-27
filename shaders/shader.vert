#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform float x_diff;
uniform float y_diff;

void main()
{
  vec4 pos = vec4(aPos, 1.0);
  pos.xy += vec2(x_diff, y_diff);
  gl_Position = pos;

  ourColor = aColor;
}
