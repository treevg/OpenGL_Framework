#version 430


uniform samplerCube tex;

in vec3 uv;
in vec4 passPosition;

uniform mat4 uniformView;

out vec4 fragColor;
out vec4 fragPosition;


void main() {

    vec3 textCoord = uv;
    fragColor  = texture(tex, textCoord);
    fragPosition = passPosition;

}
