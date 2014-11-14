#version 430

uniform float luminance;
uniform vec4 newColor;

uniform sampler2D tex2;
uniform sampler2D texOut;

in vec4 passPosition;
in vec2 passUVCoord;

layout (location = 0) out vec4 fragColor;
out vec4 fragPosition;
out vec2 passUV;
out vec3 color;

void main() {

    //fragColor = color * luminance + newColor + texture(tex2, passUVCoord);
    
    fragColor = texture(tex2, passUVCoord);
    fragPosition = passPosition;
    
    //just for testing, no urgend needing
    passUV = passUVCoord;
}