#version 430


in vec4 passPosition;
in vec2 passTextureCoordinate;

uniform sampler2D diffuse_text;




out vec4 fragColor;
out vec4 fragPosition;



void main() {

     fragPosition = passPosition;
   
     fragColor = texture(diffuse_text, passTextureCoordinate);
  
     if (fragColor == 0){

     fragColor = vec4 (0,1,1,0);
     
     }


   

}
