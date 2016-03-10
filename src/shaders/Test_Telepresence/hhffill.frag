#version 430

out vec4 fragColor;

// expanded color tex
uniform sampler2D expandedColorTexMipmap
// reduced color tex
uniform sampler2D reducedColorTexMipmap

// mipmap level
uniform int level;
// resolution of image
uniform vec2 resolution;


void main(){
	// get color at pixel coord in reduced image
	vec4 tempColor = texelFetch(reducedColorTexMipmap, gl_FragCoord.xy,  level);
	// if value color value is null (this means there is a hole in the reduced image)
	// TODO: check correctness, maybe switch black to white-check...
	if(tempColor.a == 0 && tempColor.x <= 0.0000001 && tempColor.y <= 0.0000001 && tempColor.z <= 0.0000001){
		// if temp.color is black there is a hole in the reduced version, fill it with the value at coords of expanded image
		vec4 tempColor = texelFetch(expandedColorTexMipmap, gl_FragCoord.xy,  level);
	}
	fragColor = tempColor;
}