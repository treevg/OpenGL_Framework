#version 430

// use with Filters/fullscreen.vert

uniform sampler2D tex;
uniform mat4 view;
uniform mat4 projection;

in vec4 passPosition;
out vec4 fragColor;

void main() {
	vec3 w = normalize(inverse(mat3(projection) * mat3(view)) * vec3(passPosition.xy * 2 - 1, 1));
    fragColor = texture(tex, vec2(atan(w.x, w.z) / 6.2832, acos(w.y) / 3.1416));
}