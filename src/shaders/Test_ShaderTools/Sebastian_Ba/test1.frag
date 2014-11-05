#version 430

uniform vec4 color;
uniform float luminance;
uniform vec4 newColor;

uniform sampler2D tex2;

in vec4 passPosition;
in vec2 passUVCoord;

out vec4 fragColor;
out vec4 fragPosition;
out vec2 passUV;

void main() {

    //fragColor = color * luminance + newColor + texture(tex2, passUVCoord);
    
    fragColor = texture(tex2, passUV);
    fragPosition = passPosition;
    
    //just for testing, no urgend needing
    passUV = passUVCoord;
}