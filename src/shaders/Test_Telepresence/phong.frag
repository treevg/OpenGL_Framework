#version 330 core

in vec4 passPosition;
in vec3 passNormal;

uniform vec3 lightPosition;
uniform vec3 diffuseColor; 

out vec4 fragmentColor;

void main(){
    //compute the light vector as the normalized vector between 
    //the vertex position and the light position:
    vec3 lightVector = normalize(lightPosition - passPosition.xyz);

    //compute the eye vector as the normalized negative vertex position in camera coordinates:
    vec3 eye = normalize(-passPosition.xyz);
    
    //compute the normalized reflection vector using GLSL's built-in reflect() function:
    vec3 reflection = normalize(reflect(-lightVector, normalize(passNormal)));

    //varables used in the phong lighting model:
    float phi = max(dot(passNormal, lightVector), 0);
    float psi = pow(max(dot(reflection, eye), 0), 25);

    vec3 ambientColor = vec3(0.2, 0.2, 0.2);

    fragmentColor = vec4(
       ambientColor +
       phi * diffuseColor + 
       psi,
        1);
       
}