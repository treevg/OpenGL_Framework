#version 430

uniform float interpol;
uniform sampler2D tex1;

//uniform sampler2D tex2;
//uniform sampler2D tex3;

in vec4 passPosition;

out vec4 fragColor;

void main() {
    fragColor = vec4(1,1,1,1); 
	
	//texture(tex1, passPosition.xy);
		
		//+ texture(tex2, passPosition.xy) +
	    //texture(tex3, passPosition.xy);
}