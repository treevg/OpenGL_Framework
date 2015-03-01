#version 430

uniform vec4 color;
uniform float luminance;

uniform sampler2D diffuse_text;

in vec4 passPosition;
in vec2 passTextureCoordinate;

out vec4 fragColor;
out vec4 fragPosition;


void main() {

    //fragColor = color * luminance + newColor + texture(tex2, passTextureCoordinate);
    
     fragColor = texture(diffuse_text, passTextureCoordinate);
     if (fragColor == 0){

     fragColor = vec4 (1,1,0,0);
     
     }

     fragPosition = passPosition;
   

}
