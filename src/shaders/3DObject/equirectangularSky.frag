#version 430

// use with Filters/fullscreen.vert

uniform sampler2D tex;
uniform vec2 resolution;
uniform mat4 view;
uniform mat4 projection;

in  vec3 passPosition;
out vec4 fragColor;
out vec4 fragPosition;

void main() {
	vec2 fragCoord = vec2(gl_FragCoord.xy / resolution) * 2 - 1;
	vec3 viewDir = normalize(inverse(mat3(projection) * mat3(view)) * vec3(fragCoord, 1));
        fragPosition = vec4 (passPosition.xyz, 0);
	fragColor = texture(tex, vec2(atan(viewDir.x, viewDir.z) / 6.2832 + 0.5, acos(viewDir.y) / 3.1416));
}
