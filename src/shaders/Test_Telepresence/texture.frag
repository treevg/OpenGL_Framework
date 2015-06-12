#version 430

uniform sampler2D tex;
in vec2 passUv;

out vec4 fragColor;

void main() {
	fragColor = texture(tex, passUv.xy);
	//fragColor = vec4( .0, 0.9, 0.4, 1.0f );
}