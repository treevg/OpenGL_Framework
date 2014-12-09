#version 430


uniform sampler2D color;
uniform sampler2D indirectionColor;
uniform sampler2D depth;
uniform sampler2D indirectionDepth;
uniform sampler2D fragPos;
uniform sampler2D fragPos2;

uniform int texNum;
uniform float minRange;
uniform float maxRange;


in vec4 passPosition;

out vec4 fragColor;





void main() {
 
 
	if(texNum==0){
		fragColor = texture(color, (passPosition.xy - 1) * 0.5) + texture(indirectionColor, (passPosition.xy - 1) * 0.5);
	
	}
 
	if(texNum==1){
		fragColor = texture(color, (passPosition.xy - 1) * 0.5);
	}

	if(texNum==2){
		fragColor = texture(indirectionColor, (passPosition.xy - 1) * 0.5);
		//vec2 uv = passPosition.xy;
		//float d = LinearizeDepth(uv);
		//fragColor = vec4(lin(),1);
		
		//fragColor = vec4(d, d, d, 1.0);
		
	}	
	if(texNum==3){
		fragColor = texture(depth, (passPosition.xy - 1) * 0.5);
	}
	if(texNum==4){
		fragColor = texture(indirectionDepth, (passPosition.xy - 1) * 0.5);
	}	

}