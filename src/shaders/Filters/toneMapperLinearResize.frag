#version 430

out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 resolution;
uniform float minRange = 0.0;
uniform float maxRange = 1.0;

void main() {
	vec4 c = texture(tex, gl_FragCoord.xy / resolution.xy);
	if (c.a == 0) discard;
	fragColor = vec4((c.rgb - minRange) / (maxRange - minRange), 1);
}
