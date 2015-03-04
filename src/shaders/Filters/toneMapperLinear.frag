#version 430

in vec4 passPosition;

out vec4 fragColor;

uniform sampler2D tex;

uniform float minRange;
uniform float maxRange;
uniform int level;

void main() {
	vec2 uv = passPosition.xy;
	vec3 c = texelFetch(tex, ivec2(gl_FragCoord.xy), level).xyz;
	fragColor = vec4((c - minRange) / (maxRange - minRange), 1);
}