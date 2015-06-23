#version 430

layout(location = 0) in vec4 positionAttribute;
layout(location = 1) in vec4 colorAttribute;
layout(location = 2) in vec4 instance_positionAttribute;

out vec2 texCoord;
out float depth;
out vec4 eye_pos;
out float size;

out vec4 passColor;
out vec3 passWorldNormal;
out vec4 passWorldPosition;

uniform mat4 view;
uniform mat4 projection;
uniform vec2 scale;
//uniform vec3 xyzOffset;
uniform float elapsedTime;

void main() {
    // model size is found at instance_positionAttribute.w,
    // resize it according to input
    size = instance_positionAttribute.w * scale.x;

    // expected input vertices (positionAttribute) are a quad defined by [-1..1]²
    // position defines the center of the impostor geometry
    eye_pos = view * vec4(instance_positionAttribute.xyz, 1) +
            positionAttribute * vec4(size/2, size/2, 1, 1);

    // apply offset
    int groupID = gl_InstanceID % 62;
    eye_pos = eye_pos + vec4(sin(elapsedTime + groupID * 10),cos((elapsedTime + groupID * 10)/2),sin((elapsedTime + groupID * 10)/3),0);

    // for phong lighting shader
    passWorldPosition = eye_pos;
    passWorldNormal = vec3(0,0,1);

    // fragment coordinates [-1..1]² are required in the fragment shader
    texCoord = positionAttribute.xy;

    // depth for manual depth buffer
    depth = -eye_pos.z;

    gl_Position = projection *  eye_pos;

    // color has to be transferred to the fragment shader
    passColor = colorAttribute;
}
