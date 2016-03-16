#version 330 core

in vec4 passPosition;
in vec3 passNormal;

uniform vec3 lightPosition;
uniform vec3 diffuseColor; 

out vec4 fragmentColor;

void main()
{
	// normalized vector from fragment to light
    vec3 lightVector = normalize(lightPosition - passPosition.xyz);

    // normalized vector from fragment to camera
    vec3 eye = normalize(-passPosition.xyz);
    
    //compute the normalized reflection vector using GLSL's built-in reflect() function:
    vec3 reflection = normalize(reflect(-lightVector, normalize(passNormal)));

    //variables used in the phong lighting model:
    float phi = max(dot(passNormal, lightVector), 0.1);
    float psi = pow(max(dot(reflection, eye), 0), 20);

    vec3 ambientColor = vec3(0.2, 0.2, 0.2);

	// set alpha to zero to indicate it is not the point cloud
    fragmentColor = vec4(
       ambientColor +
       phi * diffuseColor + 
       psi,
        0);
       
}