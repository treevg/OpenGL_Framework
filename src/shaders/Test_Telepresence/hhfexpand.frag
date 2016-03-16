#version 430

out vec4 fragColor;

// color tex
uniform sampler2D colorTexMipmap
// depth tex
uniform sampler2D depthTexMipmap
// mipmap level
uniform int level;
// resolution of image
uniform vec2 resolution;

// window radius (short r) (1=3x3 ,2 = 5x5, 3= 7x7...)
// uniform int windowradius;
int r = 2;

// new resolution
ivec2 newRes = ivec2(resolution * pow(0.5, level+1));

void main(){
	fragColor = vec4(0,0,0,0);
	
	vec4 tempColor = texelFetch(colorTexMipmap, coord + ivec2(x,y),  level-1);
	
	if (level > 0){
		for(int x = -r; x <= r; x++){
			for(int y = -r; y <= r; y++){
				
				// Paper says: 4 * sums of both loops above (Expanded image_(k+1) [(2m+x)/2,(2n+y)/2] )
				// only integer values are considered (is clamp correct?)
				ivec2 newCoord = clamp((ivec2(gl_FragCoord.xy * 2) + ivec2(x, y))/2, ivec2(0,0), newRes - 1);
				
			}
		}
	}
}