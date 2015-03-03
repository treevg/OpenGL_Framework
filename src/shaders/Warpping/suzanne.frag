#version 410

in vec4 passPosition;
in vec3 passNormal;
in vec2 passTextureCoordinate;
in vec3 passLightDirection;

out vec4 fragColor;
out vec4 fragPosition;

void main() {
        fragColor = vec4(passNormal,0);

	// fragColor = vec4(0.6,0.6,0.6,1);

        fragPosition = passPosition;
}
