#version 430

in vec4 passPosition;
in vec3 passColor;

out vec4 fragColor;

void main() {
	
	//if(passColor.x >= 0.97 || passColor.y >= 0.97 || passColor.z >= 0.97){
	//	fragColor = vec4(1.0,0.0,0.0, 1.0);} else{
	fragColor = vec4(passColor, 1.0);//}
	
}