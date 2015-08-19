//FRAGMENT SHADER
#version 440

in vec4 passPosition;
in vec4 passNormal;
in vec2 passUV;
in vec3 passColor;

out vec4 fragColor;

uniform mat4 model;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 materialColor;

vec3 lightPosition = vec3(viewMatrix * vec4(0, 2, 8, 1));
float intensity = 1.0;
const float PI = 3.141519;


vec3 phong(in vec3 position, in vec3 normal)
{
    
    vec3 ambientColor  = vec3(0.015);
    vec3 diffuseColor  = materialColor;
    vec3 specularColor = materialColor;
    
    vec3 lightVector      = normalize(position - lightPosition);
    vec3 eye              = normalize(-position);
    vec3 reflectionVector = normalize(reflect(lightVector, normal));

    float diffuse  = 1/PI * max(dot(lightVector, normal), 0);
    float n        = 2;
    float specular = (n + 2)/(2 * PI) * max(pow(dot(reflectionVector, eye), n), 0);

    return ambientColor + intensity * diffuse * diffuseColor + intensity * specular * specularColor;
}

void main()
{
    vec3 color = phong(passPosition.xyz, passNormal.xyz);
    fragColor = vec4(color, 1.0);
}