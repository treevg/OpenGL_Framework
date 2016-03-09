#version 430

uniform sampler2D tex;

in vec4 passPosition;
in vec4 passWorldPosition;
in vec2 passUVCoord;
in vec4 passNormal;
in vec4 passWorldNormal;

out vec4 color;
out vec4 position;
out vec4 viewPosition;
out vec4 uv;
out vec4 viewNormal;
out vec4 normal;

void main() {
    color = texture(tex, passUVCoord.xy);
    position = passWorldPosition;
    viewPosition = passPosition;
    uv = vec4(passUVCoord,0,0);
    viewNormal = passNormal;
    normal = passWorldNormal;
}
