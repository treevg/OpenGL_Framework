#version 430

//incoming data for the single textures
in vec4 passPosition;
in vec2 passUV;
//in vec3 passNormal;

uniform float red;
uniform float green;
uniform float blue;
uniform float alpha;

//writable textures for deferred screen space calculations
out vec4 positionOutput;
out vec4 colorOutput;

// layout(location = 1) out vec4 normalOutput;
// layout(location = 2) out vec4 colorOutput;
 
void main(){  
    positionOutput = passPosition;
//    normalOutput = vec4(normalize(passNormal), 0);
//    colorOutput = texture(diffuseTexture, passUV);
    colorOutput = vec4( red, green, blue, alpha );
}
