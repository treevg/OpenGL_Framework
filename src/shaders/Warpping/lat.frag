#version 430


uniform float luminance;
uniform sampler2D tex;

in vec4 passPosition;
in vec2 passUV;

out vec4 fragColor;
out vec4 fragPosition;


void main() {

   // fragColor = color * luminance + newColor + texture(tex2, passUV);
   //  fragColor = color; 
   fragColor = texture(tex, passUV);
   fragPosition = passPosition;
   

}
