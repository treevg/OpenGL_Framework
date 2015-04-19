#version 430


uniform samplerCube tex;

in vec3 passTextureCoord;
in vec4 passPosition;

out vec4 fragColor;
out vec4 fragPosition;


void main() {

    fragColor  = texture(tex, passTextureCoord);
    fragPosition = passPosition;

}
