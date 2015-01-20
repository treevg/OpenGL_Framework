#version 430

uniform sampler2D texIn;
uniform sampler2D texCbCr;
writeonly uniform image2D texY;

in vec4 passPosition;

out vec4 fragColor;

void main() {
    fragColor = texture(texIn, passPosition.xy);
    vec4 color = texture(texIn, passPosition.xy);
    
    
    
    imageStore(texY, ivec2(passPosition.xy), vec4(fragColor.ra, 0, 0));
}