#version 330 core

out vec4 FragColor;

in vec2 vUV;

uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, vUV);
}