#version 430

// use with Filters/fullscreen.vert

uniform sampler2D tex;
uniform vec2 resolution;
uniform mat4 view;
uniform mat4 projection;

in vec4 passPosition;
out vec4 fragColor;

void main() {
	vec2 fragCoord = vec2(gl_FragCoord.xy / resolution) * 2 - 1;
	vec3 viewDir = normalize(inverse(mat3(projection) * mat3(view)) * vec3(fragCoord, 1));
	fragColor = texture(tex, vec2(atan(viewDir.x, viewDir.z) / 6.2832 + 0.5, acos(-viewDir.y) / 3.1416));
}
