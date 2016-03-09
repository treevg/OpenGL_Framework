#version 430

uniform vec4 color;
uniform float blendColor;
uniform sampler2D tex;

in vec4 passPosition;
in vec2 passUVCoord;
in vec4 passNormal;
in vec4 passWorldNormal;

out vec4 fragColor;
out vec4 fragPosition;
out vec4 fragUVCoord;
out vec4 fragNormal;

void main() {
    fragColor = abs(passWorldNormal);// mix(texture(tex, passUVCoord.xy), color, blendColor);
    fragPosition = passPosition;
    fragUVCoord = vec4(passUVCoord,0,0);
    fragNormal = passNormal;
}
