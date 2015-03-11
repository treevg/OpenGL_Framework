#version 430

in vec2 passPosition;
in vec4 warpRefPos;

uniform sampler2D colorTexture;
uniform vec2 resolution;

out vec4 color;
out vec4 warpedReflectivePosition;


void main() {

color = texture(colorTexture , passPosition);
warpedReflectivePosition = warpRefPos;

}