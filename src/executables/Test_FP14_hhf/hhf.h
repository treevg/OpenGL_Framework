/*
This class implements the hierarchical hole filling for use with kinect data (rgb and depth-textures)
Used in research project FP14-Tele

This implementation consists of both a background and a foreground texture as well as a mask.

Initially a static background texture is generated.


created: bstnwlf 19.03.2015
*/

#ifndef HHF_H
#define HHF_H

#include <glm/glm.hpp>

class HHF
{
public:
	HHF();
	HHF(
		glm::vec2 fgRes,	// 
		glm::vec2 bgRes,
		glm::vec3 threshold
		
		) : fgRes(fgRes), bgRes(bgRes) {
		computeMipMapCount();
	};
	virtual ~HHF();

	void initBackgroundTexture();

	int computeMipMapCount(){
		mipmapCount = glm::log2(glm::max<float>(fgRes.x,fgRes.y));
	};

private:
	glm::vec2 fgRes;		// Foreground texture resolution
	glm::vec2 bgRes;		// Background texture resolution

//	GLuint* fgTex;			// Foreground texture
	//GLuint* bgTex;			// Background texture
		
	glm::vec3 threshold;	// Mask Threshold RGB

	int mipmapCount;		// number of mipmaps
};

#endif // HHF_H