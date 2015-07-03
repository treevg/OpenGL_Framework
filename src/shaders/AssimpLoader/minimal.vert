//VERTEX SHADER
#version 440

in vec3 pos;
in vec3 normal;
in vec2 uv;
in vec3 color;

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
    passPosition = view * model * vec4(pos, 1.0);
    passUV       = uv;
    passNormal   = transpose(inverse(view * model)) * vec4(normalize(normal), 0.0);
    passColor    = color;
    gl_Position  = projection * view * model * vec4(pos, 1);
}
