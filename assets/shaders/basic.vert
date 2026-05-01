#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 3) in mat4 aModel;

uniform mat4 u_ViewProjection;

out vec2 v_UV;

void main()
{
    v_UV = aUV;
    gl_Position = u_ViewProjection * aModel * vec4(aPos, 1.0);
}