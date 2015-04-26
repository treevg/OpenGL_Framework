#version 430

uniform sampler2D ambient_text;
uniform sampler2D diffuse_text;
uniform sampler2D specular_text;
uniform vec3 lightPos; 
uniform bool attenuatFactor;
uniform vec3 viewPosition;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;


in vec4 passPosition;
in vec2 passUV;
in vec3 passNormal;

out vec4 fragColor;
out vec4 fragPosition;


void main() {
  //get colors from the model and calculate phong lighting model
  
 vec4 ambient_color = texture(ambient_text, passUV);
 vec4 diffuse_color = texture(diffuse_text, passUV); 
 vec4 specular_color = texture(specular_text, passUV);
     

 float lightConstantAtt = 0.0f;
 float lightLinearAtt = 0.009f;
 float lightQuadraticAtt = 0.0032f;

    vec3 lightDirection = normalize(lightPos - passPosition.xyz);
    vec3 lightColor = vec3(1,1,1);
    vec3 viewVector = normalize(viewPosition-passPosition.xyz)*0.1f;
    vec3 norm = normalize(passNormal);

      //ambient shading

        vec3 ambientComponent  = Ka * ambient_color.xyz;

       /*         diffuse shading   */

      float diff = max(dot(norm, lightDirection), 0.0);
      vec3 diffuseComponen =   Kd * diff * diffuse_color.xyz;
    

       /*             specular shading        */
     vec3 reflectDirection = normalize(reflect(-lightDirection, norm));
   //  float spec = (pow(max(dot(viewVector, reflectDirection), 0.0), shinines))*0.2f;
     vec3 specularComponent =  Ks  * specular_color.xyz ;
    


      /* 		attenuation	      */
   float dist = length(lightPos - passPosition.xyz);
   float attenFactor = 1.0f / (lightConstantAtt + lightLinearAtt * dist + lightQuadraticAtt* (dist*dist));


         if(attenuatFactor) {

     ambientComponent *= attenFactor;
     specularComponent *= attenFactor;
     diffuseComponen  *= attenFactor;

           }



    fragColor = vec4(ambientComponent + specularComponent + diffuseComponen, 1.0f);
    

          if (ambient_color == 0){
 
              fragColor = vec4(1,0,0,1);
    
                    }
  
    fragPosition = passPosition;
   

}
