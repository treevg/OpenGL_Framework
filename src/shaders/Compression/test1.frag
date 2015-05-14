#version 430

uniform float luminance;
uniform vec4 newColor;

uniform sampler2D tex;
uniform sampler2D texOut;

in vec4 passPosition;
in vec2 passUVCoord;

out vec4 fragColor;
// out vec4 fragPosition;
out vec3 color;

void main() {

    //fragColor = color * luminance + newColor + texture(tex2, passUVCoord);
    
    fragColor = texture(tex, passUVCoord);
    // fragPosition = passPosition;
}