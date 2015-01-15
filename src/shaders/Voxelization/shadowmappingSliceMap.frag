#version 430

in vec2 passUV;

uniform sampler2D positionMap;
uniform sampler2D colorMap;

//uniform usampler2D shadowMap;
//uniform usampler1D bitMask;

layout(binding = 5) uniform usampler1D bitMask;
layout(binding = 6) uniform usampler2D shadowMap;	//aka slicemap

uniform mat4 lightView;
uniform mat4 lightPerspective;
uniform mat4 view;

uniform float opacityPerSlice;

out vec4 fragmentColor;

void main() {
	
	// gbuffer information
	vec4 gbufferPosition = texture(positionMap, passUV);
	if ( gbufferPosition.a != 0.0)
	{
	vec4 gbufferColor = texture(colorMap, passUV);
	
	// retrieve world positon of sample
	vec4 worldPos = inverse( view ) * ( gbufferPosition );
	
	// project into shadow map
	vec4 lightPerspPos = lightPerspective * lightView * worldPos;	
	lightPerspPos /= lightPerspPos.w;
	lightPerspPos.rgb *= 0.5;
	lightPerspPos.rgb += 0.5;
	
	// read pixel from shadow map
	vec2 shadowMapLookup   = lightPerspPos.xy ;
	uvec4 shadowMapVal = texture( shadowMap, shadowMapLookup );
	
	// amount of set slices in direction to light source
	int fullSlices = 0;					
	
	// for every slice from fragment depth (offset by one slice) to light source
	for ( float t = lightPerspPos.z; t >= 0.0; t-= 1.0 / 128.0 )
	{
		// bitmask of current slice
		uvec4 currentBitMask = texture( bitMask, t );

		// compare mask to shadow map value by bitwise AND
		if ( (currentBitMask & shadowMapVal) != uvec4(0) )
		{
			fullSlices ++;
		}
	}
	
		fragmentColor = vec4 ( gbufferColor.rgb * pow( 1.0 - ( opacityPerSlice ), fullSlices ), 1.0 );
	}
	else
	{
		fragmentColor = vec4(0,0,0,1);
	}
}