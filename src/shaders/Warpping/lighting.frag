#version 410

in vec4 passPosition;
in vec3 passNormal;
in vec2 passTextureCoordinate;



out vec4 fragColor;
out vec4 fragPosition;

uniform vec3 lightPos; 
uniform vec3 cameraPos;

uniform vec4 color;



void main() {
    vec3 lightColor = vec3(1,1,1);

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    
  	
    // Diffuse 
    vec3 norm = normalize(passNormal);
    vec3 lightDir = normalize(lightPos - vec3(passPosition.xyz));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(cameraPos -  vec3(passPosition.x, passPosition.y,passPosition.z));
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * color.xyz;
    
         fragColor = vec4(result, 1.0f);

         fragPosition = passPosition;
}
