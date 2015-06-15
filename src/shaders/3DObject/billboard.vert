#version 430

layout(location = 0) in vec4 positionAttribute;
out vec2 texCoord;
out float depth;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 position;
uniform vec2 size;

void main() {
	vec4 screenPos = view * vec4(position.xyz, 1) + positionAttribute * vec4(size.x / 2, size.y / 2, 1, 1);
	texCoord = positionAttribute.xy;
	depth = -screenPos.z;
	gl_Position = projection *  screenPos;
}