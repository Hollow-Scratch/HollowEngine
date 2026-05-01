#version 330 core

out vec4 FragColor;

uniform float u_Time;

void main()
{
    float r = 0.6 + 0.2 * sin(u_Time * 0.5);
    float g = 0.6 + 0.2 * sin(u_Time * 0.5 + 2.0);
    float b = 0.6 + 0.2 * sin(u_Time * 0.5 + 4.0);

    FragColor = vec4(r, g, b, 1.0);
}