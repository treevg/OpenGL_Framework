#version 430

out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 resolution;
uniform float minRange = 0.0;
uniform float maxRange = 1.0;
uniform int level = 0;

void main() {
	vec2 fragCoord = vec2(gl_FragCoord.xy / resolution);
	vec3 c = texture(tex, fragCoord, level).xyz;
	fragColor = vec4((c - minRange) / (maxRange - minRange), 1);
}