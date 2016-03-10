#version 430

out vec4 fragColor;

// color tex
uniform sampler2D colorTexMipmap
// depth tex
uniform sampler2D depthTexMipmap
// mipmap level
uniform int level;

// window radius (short r) (1=3x3 ,2 = 5x5, 3= 7x7...)
// uniform int windowradius;
int r = 2;

void main(){
	// init color black
	fragColor = vec4(0,0,0,0);

	// max count
	int countmax = (r*2 +1) * (r*2 +1);
	
	// if top level
	if (level == 0){
		// get current position
		ivec2 newCoord = ivec2(gl_FragCoord.xy);
		// leave frag color unchanged
        fragColor = vec4(texelFetch(mipmapTexture, newCoord, 0).xyz,1);
	} else { // if not top level
		// count non-zero pixel
		float count = 0;
		// frag coordinate from upper level
		ivec2 ulcoord = ivec2(gl_FragCoord.xy)*2;
		
		// iterate over upper level picture
		for(int x = -r; x <= r; x++){
			for(int y = -r; y <= r; y++){
				// get color from neighbor pixel
				vec4 tempColor = texelFetch(colorTexMipmap, coord + ivec2(x,y),  level-1);
				// check if pixel is not zero
				if(tempColor.a > 0 && (tempColor.x > 0.0000001 || tempColor.y > 0.0000001 || tempColor.z > 0.0000001)
				&& !isnan(tempColor.x)){
					// increment counter
					count += 1;
					// add color to tempcolor, will be divided / number
					fragColor += tempColor;
				}
			}
		}
		
		if (count > 0 && count < countmax){
			fragColor /= count;
		}else if (count == countmax){
		// use gaussian kernel(x,y) * fragment(x,y)
		//TODO: implement
		} else { // count <= 0
			fragColor = vec4(0,0,0,0);
		}
	// bad stuff happend if count > countmax...
}