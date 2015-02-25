#version 430

// use with Filters/fullscreen.vert

uniform sampler2D tex;
uniform mat4 view;
uniform mat4 projection;

in vec4 passPosition;
out vec4 fragColor;

void main() {
	vec3 dir = normalize(inverse(mat3(projection) * mat3(view)) * vec3(passPosition.xy * 2 - 1, 1));
    fragColor = texture(tex, vec2(atan(dir.x, dir.z) / 6.2832 + 0.5, acos(dir.y) / 3.1416));
}