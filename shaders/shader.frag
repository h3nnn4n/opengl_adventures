#version 330 core
out vec4 FragColor;
in vec3 ourColor;

uniform float redOverride;
uniform float time;

void main()
{
    FragColor = vec4(ourColor, 1.0) + vec4(redOverride, 0.0, 0.0, 0.0) * sin(time);
}
