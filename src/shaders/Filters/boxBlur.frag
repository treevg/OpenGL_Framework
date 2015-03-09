#version 430

// attention, this is a dumb and slow method for presentation purpose only

in vec3 passPosition;

out vec4 fragColor;

uniform sampler2D tex;

uniform vec2 resolution;
uniform int strength;

void main() {
	vec2 delta = vec2(1.0) / resolution;

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