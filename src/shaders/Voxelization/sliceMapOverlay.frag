#version 430

in vec2 passUV;

uniform sampler2D baseTexture;

uniform float backgroundTransparency;

layout( rgba32ui, binding = 0 ) uniform readonly uimage2D sliceMapTexture;

out vec4 fragmentColor;

void main() {
	
	vec4 sliceAdd = vec4(0.0, 0.0, 0.0, 0.0);

	// size
	ivec2 sliceTexSize = imageSize( sliceMapTexture );

	// load texel
	ivec2 sliceCoords = ivec2( passUV * vec2( sliceTexSize ) );
	uvec4 sliceTex = imageLoad( sliceMapTexture, sliceCoords );
	
	// map unsigned int values to [0.0 .. 1.0] float range
	float r = float(sliceTex.r);
	float g = float(sliceTex.g);
	float b = float(sliceTex.b);
	float a = float(sliceTex.a);
	
	if (r != 0) {
		r = log2(r) / 32.0;
	}
	if (g != 0) {
		g = log2(g) / 32.0;
	}
	if (b != 0) {
		b = log2(b) / 32.0;
	}
	if (a != 0) {
		a = log2(a) / 32.0;
	}

	// alpha is distributed among r,g,b channels --> white
	sliceAdd = vec4(r + a, g + a, b + a, 0.0);	
	
	// base color
	vec4 baseTex = texture(baseTexture, passUV) * ( 1.0 - min( 1.0, max( 0.0, backgroundTransparency ) ) );
	
	// add
	fragmentColor = vec4 ( baseTex.rgb + sliceAdd.rgb * backgroundTransparency, 1.0);
}