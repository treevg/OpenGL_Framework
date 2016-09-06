#version 430

out vec4 fragColor;

uniform sampler2D tex;
uniform float minRange = 0.0;
uniform float maxRange = 1.0;
uniform int level = 0;

void main() {
	vec4 c = texelFetch(tex, ivec2(gl_FragCoord / pow(2,level)), level);
	if (c.a == 0) discard;
	fragColor = vec4((c.rgb - minRange) / (maxRange - minRange), 1);
}
