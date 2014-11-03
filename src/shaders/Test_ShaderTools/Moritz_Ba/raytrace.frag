#version 430


uniform vec4 color;
uniform vec4 sphere1;
uniform float luminance;
uniform mat4 mvp;

in vec4 passPosition;

out vec4 fragColor;
out vec4 fragColor2;
out vec4 fragPosition;

void main() {
	mat4 invMVP = inverse(mvp);
	vec4 pixelPos = invMVP * vec4(passPosition.xy, 1, 1);
	vec4 cameraPos = invMVP * vec4(0,0,0,1);
	vec4 ray = pixelPos - cameraPos;

	float lambda; 
	//für welches lambda gilt:
	// cameraPos + ray * lambda == sphere intersect


	fragColor = passPosition;


    fragColor2 = color * luminance + sphere1;
    fragPosition = passPosition;
}