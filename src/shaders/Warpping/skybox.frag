#version 430


uniform samplerCube tex;

in vec3 uv;

out vec4 fragColor;


void main() {

    fragColor  = texture(tex, uv);

}
