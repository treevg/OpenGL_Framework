#version 430

uniform vec4 color;
uniform float luminance;


uniform sampler2D tex;

in vec4 passPosition;
in vec2 passUV;

out vec4 fragColor;
out vec4 fragPosition;


void main() {

   // fragColor = color * luminance + newColor + texture(tex2, passUV);
    
   fragColor = texture(tex, passUV);
    //  fragColor = color;
      fragPosition = passPosition;
   

}
