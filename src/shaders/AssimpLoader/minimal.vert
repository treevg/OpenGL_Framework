#version 330

in vec4 pos;
in vec2 uv;

uniform float scale;
out vec4 passPosition;
out vec2 passUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    passPosition = projection * view * model * pos;
    passUV = uv;
    gl_Position = projection * view * model * vec4(pos.xyz, 1);
}
