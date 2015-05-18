#version 430

uniform sampler2D ambient_text;
uniform sampler2D diffuse_text;
uniform sampler2D specular_text;

uniform vec3 lightPos; 

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;


in vec4 passPosition;
in vec2 passUV;
in vec3 passNormal;
in float visability;

out vec4 fragColor;
out vec4 fragPosition;




void main() {
  //get colors from the model and calculate phong lighting model
  
 vec4 ambient_color = texture(ambient_text, passUV);
 vec4 diffuse_color = texture(diffuse_text, passUV); 
 vec4 specular_color = texture(specular_text, passUV);

 vec3 ambient_light = vec3(0.79,0.8,0.74);
 vec3 diffuse_light = vec3(0.5,0.5,0.5); 

//diffuse

 vec3 norm = normalize(passNormal);
 vec3 lightDirection = normalize(lightPos - passPosition.xyz);
 float diff = max(dot(norm, lightDirection), 0.0);
 vec3 diffuseComponent =  diff * Kd * diffuse_light;

//ambient shading

  vec3 ambientComponent  = Ka * ambient_light;

  fragColor = vec4(ambientComponent + diffuseComponent, 1.0f)*diffuse_color;
   vec4 skycolor = vec4(0.2,0.2,0.2,1);
  fragColor = mix(skycolor, fragColor, visability);
 
    
fragPosition = passPosition;
   

}
