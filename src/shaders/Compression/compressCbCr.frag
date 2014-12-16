#version 430

uniform sampler2D texIn;
uniform sampler2D texCbCr;
uniform sampler2D texY;

in vec4 passPosition;

out vec4 fragColor;

void main() {
    fragColor = texture(texIn, passPosition.xy);
    vec4 color = texture(texIn, passPosition.xy);
    
    //imageStore(texY, passPosition.xy, vec4(fragColor.ra, 0, 0));
}