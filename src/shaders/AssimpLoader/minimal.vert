#version 440

layout (location=0) in vec3 pos;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 uv;
layout (location=3) in vec3 color;

out vec4 passPosition;
out vec2 passUV;
out vec4 passNormal;
out vec3 passColor;

uniform float scale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    passPosition = model * vec4(pos, 1.0);
    passUV       = uv;
    passNormal   = inverse(transpose(view * model)) * vec4(normal, 0.0);
    passColor    = color;
    gl_Position  = projection * view * model * vec4(pos, 1);
}
