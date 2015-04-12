#version 430

in vec4 passPosition;
in vec3 passNormal;
in vec2 passTextureCoordinate;

uniform vec3 lightPos; 
uniform sampler2D diffuse_text;


out vec4 fragColor;
out vec4 fragPosition;




void main() {
    vec3 lightDirection = normalize(lightPos - passPosition.xyz);
    vec3 lightColor = vec3(1,1,1);
    vec3  viewVector = normalize(-passPosition.xyz);

  
    vec3 ambient = vec3(0.3,0.2,0.2);

  	
    // Diffuse 
    vec3 norm = normalize(passNormal);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 1.0f;
   
  // vec3 viewDir = normalize(cameraPos -  vec3(passPosition.x, passPosition.y, passPosition.z));

    vec3 reflectDirection = normalize(reflect(-lightDirection, norm)); 


    float specFact = pow(max(dot(reflectDirection, viewVector), 0.0), 15);
    

    vec3 specular = specularStrength * specFact * lightColor;  
    vec4 colorTemp = texture(diffuse_text, passTextureCoordinate);
    vec3 result = (ambient + diffuse + specular) * colorTemp.xyz;
    
         fragColor = vec4(result, 1.0f);

         fragPosition = passPosition;
}
