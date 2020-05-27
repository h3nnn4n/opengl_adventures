#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform float x_diff;
uniform float y_diff;

void main()
{
  vec4 pos = vec4(aPos, 1.0);
  pos.xy += vec2(x_diff, y_diff);
  gl_Position = pos;

  ourColor = aColor;
  TexCoord = aTexCoord;
}
