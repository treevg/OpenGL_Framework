#version 430

uniform float luminance;
uniform sampler2D ambient_text;
uniform sampler2D diffuse_text;
//uniform sampler2D specular_text;


in vec4 passPosition;
in vec2 passUV;

out vec4 fragColor;
out vec4 fragPosition;


void main() {
  //get color from the model and calculate fragColor
  

 vec4 ambient_color = texture(ambient_text, passUV);
 vec4 diffuse_color = texture(diffuse_text, passUV); 
// vec4 specular_color = texture(specular_text, passUV);



fragColor = ambient_color*0.5 + diffuse_color;

if (fragColor == 0){
 
fragColor = vec4(1,1,0,1);
    
}
  
 fragPosition = passPosition;
   

}
