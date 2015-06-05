#version 430

in vec2 texCoord;
in float depth;
out vec4 fragColor;

uniform vec2 size;
uniform sampler2D tex;

void main() {
	vec4 texel = texelFetch(tex, ivec2(gl_FragCoord), 0);
	if (length(texCoord) > 1) discard;

	float curvature = (sin(acos(length(texCoord.xy) * 3.141 / 2))) / 3.141 * 2 * min(size.x, size.y) / 2;
	float modifiedDepth = depth - curvature; 
	
	if (modifiedDepth < texel.w)
		fragColor = vec4(1/modifiedDepth,1/modifiedDepth,1/modifiedDepth, modifiedDepth);
	else 
		fragColor = texel;
}