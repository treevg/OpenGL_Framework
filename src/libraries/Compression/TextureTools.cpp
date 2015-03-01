#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "stb_image.h"
#include "TextureTools.h"

namespace TextureTools {
    GLuint loadTexture(std::string fileName){
      
        int width, height, bytesPerPixel;

        std::string new_name;
        
        new_name.append(RESOURCES_PATH);
        new_name.append("/");  
        new_name.append(fileName);

        unsigned char *data = stbi_load(new_name.c_str(), &width, &height, &bytesPerPixel, 0);

        //create new texture
        GLuint textureHandle;
        glGenTextures(1, &textureHandle);
     
        //bind the texture
        glBindTexture(GL_TEXTURE_2D, textureHandle);
     
        //send image data to the new texture
        if (bytesPerPixel < 3) {
            std::cout << "ERROR: Unable to open image "  << new_name << std::endl;
            return -1;
        } else if (bytesPerPixel == 3){
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (bytesPerPixel == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "Unknown format for bytes per pixel... Changed to \"4\"" << std::endl;
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }

        //texture settings
        glGenerateMipmap(GL_TEXTURE_2D);

         if (fileName.find("repeating") != std::string::npos ){
            std::cout << "repeating texture " << std:: endl;

           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
           glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

         }
         
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        std::cout << "SUCCESS: image loaded from " << new_name << std::endl;
        return textureHandle;
    }
}