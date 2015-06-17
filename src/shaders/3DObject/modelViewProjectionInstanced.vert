#version 430

layout(location = 0) in vec4 positionAttribute;
layout(location = 1) in vec2 uvCoordAttribute;
layout(location = 2) in vec4 normalAttribute;
layout(location = 4) in vec4 colorAttribute;
layout(location = 5) in vec4 instance_positionAttribute;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float scale;

out vec4 passWorldPosition;
out vec4 passPosition;
out vec2 passUVCoord;
out vec3 passWorldNormal;
out vec3 passNormal;

out vec4 passColor;

void main(){
    passUVCoord = uvCoordAttribute;

    vec4 modelPos = positionAttribute  * scale;
    passWorldPosition = modelPos + instance_positionAttribute;
    passPosition = view * vec4(instance_positionAttribute.xyz, 1)
            + modelPos;
    passWorldNormal = ( transpose( inverse( model ) ) * normalAttribute).xyz;
    passNormal = ( transpose( inverse( view * model ) ) * normalAttribute ).xyz;

    gl_Position =  projection * passPosition;
    passColor = colorAttribute;
}
