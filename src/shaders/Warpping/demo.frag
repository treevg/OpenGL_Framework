#version 430


in vec4 passPosition;
in vec2 passTextureCoordinate;

uniform sampler2D diffuse_text;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;




out vec4 fragColor;
out vec4 fragPosition;



void main() {

     fragPosition = passPosition;
   
     vec4 temporColor= texture(diffuse_text, passTextureCoordinate);

     temporColor = temporColor + vec4(Ka.xyz,1) + vec4(Ks.xyz,1) + vec4(Kd.xyz,1);

     fragColor = temporColor;
  
     if (fragColor == 0){

     fragColor = vec4 (0,1,1,0);
     
     }


   

}
