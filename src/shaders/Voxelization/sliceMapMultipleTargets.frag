#version 430 

// a fragment shader which sets the corresponding 
// pixel bit voxel in the slice map to 1

in float clipDepth; // relative position between near and far plane

uniform sampler2D bitMask;
uniform int numSlicemaps;	// number of slicemaps to accumulate (max 4)

// out : layout positions for multiple render targets
layout(location = 0) out vec4 slice0_127;

// (optional) use multiple render targets
layout(location = 1) out vec4 slice128_255;
layout(location = 2) out vec4 slice256_383;
layout(location = 3) out vec4 slice384_511;
 
void main() { 
	float z = clipDepth;
	
	// determine slicemap to write to
	int sliceMapTarget = int ( numSlicemaps * z ); 
	
	// map from z to [0..1] in slice map target
	z = ( z * float( numSlicemaps ) ) - float( sliceMapTarget ); 

//TODO can be ignored since bitmask holds all color informations	
//	int channel = int(z * 4.0f);  				// determine color channel to write to
//	z = ( z * 4.0f ) - float( channel );		// determine sample position from 1D texture
	
	// bit mask lookup determines bit value
	vec4 bit_value = texture( bitMask, vec2( 0.5,z  ) );
	
	//TODO use usampler
	
	if (sliceMapTarget == 0)
	{
		slice0_127 = bit_value;	
	}
	if (sliceMapTarget == 1)
	{
		slice128_255 = bit_value;
	}
	if (sliceMapTarget == 2)
	{
		slice256_383 = bit_value;
	}
	if (sliceMapTarget == 3)
	{
		slice384_511 = bit_value;
	}
}