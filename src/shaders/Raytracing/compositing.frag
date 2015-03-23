#version 430

in vec4 passPosition;

uniform sampler2D gatherRefColTexture;
uniform sampler2D warpDiffColTexture;
//uniform sampler2D depth;
//uniform sampler2D indirectionDepth;

out vec4 fragColor;

void main() {
 fragColor = vec4(0);
fragColor = texture(gatherRefColTexture, (passPosition.xy-1)*0.5);
if(fragColor == vec4(0)){
fragColor = texture(warpDiffColTexture, (passPosition.xy-1)*0.5);
}
fragColor += vec4(0.2,0.2,0.2, 0.0);
}