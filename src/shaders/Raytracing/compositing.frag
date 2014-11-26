#version 430

//uniform float interpol;
uniform sampler2D tex1;

//uniform sampler2D tex2;
//uniform sampler2D tex3;

in vec4 passPosition;

in vec4 fragPosition;

out vec4 fragColor;

void main() {
 fragColor = vec4(0,1,1,1); 
	//fragColor = texture(tex1, passPosition.xy);
	//fragColor = texture(tex1, fragPosition.xy);
		
		//+ texture(tex2, passPosition.xy) +
	    //texture(tex3, passPosition.xy);
}