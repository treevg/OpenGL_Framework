#version 430

in vec3 passPosition;

out vec4 fragColor;

uniform sampler2D tex;

uniform int width;
uniform int height;
uniform int strength;

void main() {
	vec2 delta = vec2(1.0 / float(width), 1.0 / float(height));

	vec4 color = vec4(0,0,0,0);
	float i = 0;
	for (int x = -strength; x <= strength; x++) {
		for (int y = -strength; y <= strength; y++) {
			i++;
			color += texture(tex, passPosition.xy + delta * vec2(x,y));
		}
	}
	color /= i;
	fragColor = vec4(color);
}