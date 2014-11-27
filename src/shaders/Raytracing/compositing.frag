#version 430


uniform sampler2D color;
uniform sampler2D indirectionColor;
uniform sampler2D depth;
uniform sampler2D indirectionDepth;
uniform int texNum;

in vec4 passPosition;

out vec4 fragColor;

void main() {

if(texNum==0){
	fragColor = texture(color, passPosition.xy);
	
}
 
if(texNum==1){
	fragColor = texture(indirectionColor, passPosition.xy);
}

if(texNum==2){
	fragColor = texture(depth, passPosition.xy);
}	
if(texNum==3){
	fragColor = texture(indirectionDepth, passPosition.xy);
}	

}