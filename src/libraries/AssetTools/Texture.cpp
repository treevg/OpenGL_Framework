//#include "Texture.h"
//
//Texture::Texture(std::string path) {
//
//	textureHandle = load(path);
//}
//
//Texture::~Texture() {
//
//}
//
//GLuint Texture::getHandle() {
//	return textureHandle;
//}
//
//GLuint Texture::load(std::string path) {
//	if (!devILInitialized) {
//		ilInit();
//		iluInit();
//		ilutRenderer(ILUT_OPENGL);
//		devILInitialized = true;
//	}
//
//    ILuint iid;
//    ilGenImages(1, &iid);
//    ilBindImage(iid);
//    ilLoadImage(path.c_str());
//
//    int w = ilGetInteger(IL_IMAGE_WIDTH);
//    int h = ilGetInteger(IL_IMAGE_HEIGHT);
//    const int byteCount = sizeof(float) * 3 * w * h;
//    unsigned char* pixels = new unsigned char[byteCount];
//    ilCopyPixels(0, 0, 0, w, h, 1, IL_RGB, IL_FLOAT, pixels);
//
//    GLuint t;
//    glGenTextures(1, &t);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, t);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGB, GL_FLOAT, pixels);
//    glBindImageTexture(0, t, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);
//
//    return t;
//}
