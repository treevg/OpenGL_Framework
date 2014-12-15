#version 430

in vec2 passUV;

uniform sampler2D baseTexture;

uniform float backgroundTransparency;

//uniform bool enabled;

layout( rgba32ui, binding = 0 ) uniform readonly uimage2D sliceMapTexture;

out vec4 fragmentColor;

void main() {
	
	vec4 sliceAdd = vec4(0.0, 0.0, 0.0, 0.0);
	
//	if (enabled)
//	{
		// size
		ivec2 sliceTexSize = imageSize( sliceMapTexture );
	
		// load texel
		uvec4 sliceTex = imageLoad( sliceMapTexture, ivec2( passUV * vec2(sliceTexSize) ) );

		float r = float(sliceTexSize.x);		
		float g = float(sliceTexSize.y);  
		float b = 0.0;  
		float a = 0.0;  
		
		// alpha is distributed among r,g,b channels --> white
		sliceAdd = vec4( r + a , g + a, b + a, 1.0);	
//	}
	
	// base color
	vec4 baseTex = texture(baseTexture, passUV) * ( 1.0 - min( 1.0, max( 0.0, backgroundTransparency ) ) );
	
	// add
	fragmentColor = vec4 ( baseTex.rgb + sliceAdd.rgb, 1.0);
}