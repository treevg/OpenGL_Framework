#version 430

in vec4 passPosition;

out vec4 fragColor;

uniform sampler2D tex;

uniform float minRange = 0;
uniform float maxRange = 1;

void main() {
	vec2 uv = passPosition.xy;
	vec3 c = texture(tex, passPosition.xy).xyz;
	fragColor = vec4((c - minRange) / (maxRange - minRange), 1);
}