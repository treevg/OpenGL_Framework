#version 430

out vec4 fragColor;
 in vec4 passPosition;
// color tex
uniform sampler2D m_pcOutputTex;
// depth tex
//uniform sampler2D depthTexMipmap
// mipmap level
uniform int m_hhfMipmapLevel;

uniform int m_hhfResolution;

ivec2 m_hhfNewRes = ivec2(m_hhfResolution * pow(0.5, m_hhfMipmapLevel+1));

// window radius (short r) (1=3x3 ,2 = 5x5, 3= 7x7...)
// uniform int windowradius;
int r = 2;
int countmax = (r*2 +1) * (r*2 +1);
int count;

void main(){
	// init color black
	fragColor = vec4(0,0,0,0);
	
	// count non-zero pixel
	count = 0;
	
	// if top level
	if (m_hhfMipmapLevel == 0){
		// get current position
		ivec2 newCoord = ivec2( gl_FragCoord.xy);
		//ivec2 newCoord = ivec2(gl_FragCoord.x/m_hhfNewRes.x,gl_FragCoord.y/m_hhfNewRes.y);
		// leave frag color unchanged
		
		fragColor = texelFetch(m_pcOutputTex, newCoord,0);
	} else { // if not top level
		
		// frag coordinate from upper level
		//ivec2 ulcoord = ivec2((passPosition.xy + 1.0)*0.5);
		ivec2 ulcoord = ivec2(gl_FragCoord.xy)*2;
		// iterate over upper level picture
		for(int x = -r; x <= r; x++){
			for(int y = -r; y <= r; y++){
				// get color from neighbor pixel
				vec4 tempColor =  texelFetch(m_pcOutputTex, ulcoord+ivec2(x,y) , m_hhfMipmapLevel-1);
				//vec4 tempColor = textureLod(m_pcOutputTex, ulcoord+ivec2(x,y), m_hhfMipmapLevel-1);
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
			//fragColor = vec4(0,1,0,1);
		}else if (count == countmax){
		// use gaussian kernel(x,y) * fragment(x,y)
		//TODO: implement
			fragColor /= count;
			//fragColor = vec4(0,0,1,1);
		} else { // count <= 0
			fragColor = vec4(1,0,0,1);
		}
	}
	

		
	// bad stuff happend if count > countmax...
}

/*

uniform sampler2D m_pcOutputTex;

 in vec4 passPosition;

 out vec4 fragColor;

 void main() {
   //fragColor = texture(m_pcOutputTex, gl_FragCoord.xy);
	  fragColor = texture(m_pcOutputTex, (passPosition.xy + 1.0)*0.5);

	//fragColor = vec4(1.0,0,0,1.0);
}

*/
