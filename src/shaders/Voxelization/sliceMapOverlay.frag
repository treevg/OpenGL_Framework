#version 430

in vec2 passUV;

uniform sampler2D baseTexture;

uniform float backgroundTransparency;

layout( rgba32ui, binding = 1 ) uniform readonly uimage2D sliceMapTexture;

out vec4 fragmentColor;

void main() {
	
	vec4 sliceAdd = vec4(0.0, 0.0, 0.0, 0.0);

	// size
	ivec2 sliceTexSize = imageSize( sliceMapTexture );

	// load texel
	ivec2 sliceCoords = ivec2( passUV * vec2( sliceTexSize ) );
	
	uvec4 sliceTex = imageLoad( sliceMapTexture, sliceCoords );
	
//	float r = float(log2(sliceTex.r));
	float r = float(sliceTex.r);
	
	if ( sliceTex.r == 0 && sliceTex.g == 0 && sliceTex.b == 0 && sliceTex.a == 0)
	{
		r = sliceCoords.x / 1280.0;		
	}
	
//	float g = float(log2(sliceTex.g)) / 32.0;
//	float b = float(log2(sliceTex.b)) / 32.0;
//	float a = float(log2(sliceTex.a)) / 32.0;

//		float r =  0.3;		
	float g = 0.1;
	float b = 0.1;
	float a = 0.0;

	// alpha is distributed among r,g,b channels --> white
	sliceAdd = vec4(r + a, g + a, b + a, 1.0);	
	
	// base color
	vec4 baseTex = texture(baseTexture, passUV) * ( 1.0 - min( 1.0, max( 0.0, backgroundTransparency ) ) );
	
	// add
	fragmentColor = vec4 ( baseTex.rgb + sliceAdd.rgb, 1.0);
}