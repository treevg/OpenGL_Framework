#version 430

in vec2 passUV;

uniform sampler2D positionMap;
uniform sampler2D colorMap;

//uniform usampler2D shadowMap;
//uniform usampler1D bitMask;

layout(binding = 5) uniform usampler1D bitMask;  // accumulated bitmask
layout(binding = 6) uniform usampler2D shadowMap;// aka slice map

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
		lightPerspPos.xyz /= lightPerspPos.w;
		lightPerspPos.xyz *= 0.5;
		lightPerspPos.xyz += 0.5;
		
		// read pixel from shadow map
		vec2 shadowMapLookup   = lightPerspPos.xy ;
		float t = lightPerspPos.z - (1.0 / 128.0); // 1 slice offset to prevent self-occlusion
		
		int fullSlices = 0;
		if ( t >= 0.0 && t < 1.0 )
		{
			// truncate bits behind slice
			uvec4 mask = texture( bitMask, t);
			uvec4 shadowMapVal = texture( shadowMap, shadowMapLookup );
			uvec4 slicesToLight = mask & shadowMapVal;
		
			// amount of set slices in direction to light source
			fullSlices = int( dot( bitCount(slicesToLight), uvec4(1,1,1,1) ) );
		}		
		fragmentColor = vec4 ( gbufferColor.rgb * pow( 1.0 - ( opacityPerSlice ), fullSlices ), 1.0 );
	}
	else
	{
		fragmentColor = vec4(0,0,0,1);
	}
}