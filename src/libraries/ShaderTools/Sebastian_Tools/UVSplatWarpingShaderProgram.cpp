#include "UVSplatWarpingShaderProgram.h"

namespace RemoteGI {


UVSplatWarpingShaderProgram::UVSplatWarpingShaderProgram(int width, int height)
: width(width), height(height){
    generateProgram();
    generateTexture();
}

UVSplatWarpingShaderProgram::~UVSplatWarpingShaderProgram() {

}

void UVSplatWarpingShaderProgram::generateProgram() {
    // Creating the compute shader, and the program object containing the shader
    shaderProgramHandle = glCreateProgram();
    GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

    // In order to write to a texture, we have to introduce it as image2D.
    // local_size_x/y/z layout variables define the work group size.
    // gl_GlobalInvocationID is a uvSplatec3 variable giving the global ID of the thread,
    // gl_LocalInvocationID is the local index within the work group, and
    // gl_WorkGroupID is the work group's index

    std::string w = std::to_string(width);
    std::string h = std::to_string(height);

    std::string code = "#version 430 \n\
layout ( rgba16f, binding = 0 ) uniform image2D outTexture;\n\
layout ( rgba16f, binding = 2 ) uniform image2D positionTexture;\n\
\n\
uniform mat4 mvpOld;\n\
uniform mat4 mvpNew;\n\
\n\
layout (local_size_x = 16, local_size_y = 16) in;\n\
void main() {\n\
    ivec2 inCoord = ivec2(gl_GlobalInvocationID.xy);\n\
    vec4 wPos = imageLoad(positionTexture, inCoord);\n\
    vec4 iPos = mvpNew * vec4(wPos.xyz,1);\n\
    iPos /= iPos.w;\n\
    vec2 coordColor = vec2(\n\
        float(float(inCoord.x) / " + w + ".0),\n\
        float(float(inCoord.y) / " + h + ".0));\n\
    ivec2 outCoord = ivec2(\n\
        float(iPos.x + 1.0) * 0.5 * " + h + ".0 + (" + w + ".0-" + h + ".0)/2.0,\n\
        float(iPos.y + 1.0) * 0.5 * " + h + ".0);\n\
    vec4 lastIteration;\n\
    ivec2 tmp;\n\
    tmp = outCoord;\n\
    lastIteration = imageLoad(outTexture, tmp);\n\
    if (lastIteration.z > iPos.z) {\n\
        imageStore(outTexture, tmp, vec4(coordColor, iPos.z, 1.0));\n\
    }\n\
}\n";

    const char* c_code = code.c_str();
    glShaderSource(cs, 1, &c_code, NULL);
    glCompileShader(cs);
    int rvalue;
    glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
    if (!rvalue) {
        fprintf(stderr, "Error in compiling the compute shader\n");
        GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(cs, 10239, &length, log);
        fprintf(stderr, "Compiler log:\n%s\n", log);
        exit(40);
    }
    glAttachShader(shaderProgramHandle, cs);

    glLinkProgram(shaderProgramHandle);

    mvpOldLocation = glGetUniformLocation(shaderProgramHandle, "mvpOld");
    mvpNewLocation = glGetUniformLocation(shaderProgramHandle, "mvpNew");

    glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &rvalue);
    if (!rvalue) {
        fprintf(stderr, "Error in linking compute shader program\n");
        GLchar log[10240];
        GLsizei length;
        glGetProgramInfoLog(shaderProgramHandle, 10239, &length, log);
        fprintf(stderr, "Linker log:\n%s\n", log);
        exit(41);
    }   
    glUseProgram(shaderProgramHandle);

    checkErrors("Compute shader");
}

void UVSplatWarpingShaderProgram::generateTexture() {
    glGenFramebuffers(1, &frameBufferObjectHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);

    glGenTextures(1, &outTextureHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outTextureHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    // Allocate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    checkErrors("Gen texture"); 
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTextureHandle, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void UVSplatWarpingShaderProgram::run(GLuint positionTexture, glm::mat4 mvpOld, glm::mat4 mvpNew, int iterations) {
    glUseProgram(shaderProgramHandle);

    glUniformMatrix4fv(mvpOldLocation, 1, GL_FALSE, glm::value_ptr(mvpOld));
    glUniformMatrix4fv(mvpNewLocation, 1, GL_FALSE, glm::value_ptr(mvpNew));

    glBindImageTexture(0, outTextureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA16F);
    glBindImageTexture(2, positionTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA16F);

    clearOutTexture();
    for (int i = 0; i < iterations; i++) {
        glDispatchCompute(width/16, height/16, 1); // 512^2 threads in blocks of 16^2
    }

    checkErrors("Dispatch compute shader");
    glMemoryBarrier( GL_SHADER_STORAGE_BARRIER_BIT );
}

void UVSplatWarpingShaderProgram::clearOutTexture() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObjectHandle);
    GLfloat clearColor[4] = {9999, 9999, 9999, 0};
    glClearBufferfv(GL_COLOR, 0, clearColor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}
