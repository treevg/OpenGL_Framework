#version 440

in vec4 passPosition;
in vec2 passUV;
in vec4 passNormal;
in vec3 passColor;

out vec4 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 lightPosition = vec3(0, 8, 0);

vec3 lambert(in vec3 position, in vec3 normal)
{
    vec3 ambientColor = vec3(0.25);
    vec3 lightVector = normalize(position - lightPosition);
    float diffuse = max(dot(lightVector, normal), 0.0);
    vec3 diffuseColor = diffuse * vec3(1.0);
    return ambientColor + diffuseColor;
}

void main()
{
    fragColor = vec4(lambert(passPosition.xyz, passNormal.xyz), 1.0);
}