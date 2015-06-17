#version 430

layout(location = 0) in vec4 positionAttribute;
out vec2 texCoord;
out float depth;
out vec4 eye_pos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 position;
uniform vec2 size;

void main() {
    // expected input vertices (positionAttribute) are a quad defined by [-1..1]²
    // position defines the center of the impostor geometry
    eye_pos = view * vec4(position.xyz, 1) + positionAttribute * vec4(size.x / 2, size.y / 2, 1, 1);
    // fragment coordinates [-1..1]² are required in the fragment shader
    texCoord = positionAttribute.xy;
    // depth for manual depth buffer
    depth = -eye_pos.z;
    gl_Position = projection *  eye_pos;
}
