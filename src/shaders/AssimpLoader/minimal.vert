#version 330

in vec4 pos;
in vec2 uv;

uniform float scale;
out vec4 passPosition;
out vec2 passUV;

void main()
{
    mat4 rot = mat4(cos(45),-sin(45),  0, 0,
                    sin(45), cos(45),  0, 0,
                    0,       0,        1, 0,
                    0,       0,        0, 1);
    passPosition = rot * pos;
    passUV = uv;
    gl_Position = rot * vec4(pos.xyz, 1);
}
