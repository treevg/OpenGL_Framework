#version 430 

// a fragment shader which sets the corresponding 
// pixel bit voxel in the slice map to 1

in float clipDepth; // depth position between near and far plane

uniform usampler1D bitMask;

// out : layout positions for multiple render targets
layout(location = 0) out vec4 slice0_31;

// use multiple render targets
layout(location = 1) out vec4 slice32_63;
layout(location = 2) out vec4 slice64_93;
layout(location = 3) out vec4 slice94_127;
 
void main() { 
	float z = clipDepth;
	
	int sliceMapTarget = int ( 4 * z );			// slice map to write into
	z = ( z * 4.0f ) - float( sliceMapTarget ); // map from z to [0..1] in slice map target
	int channel = int(z * 4.0f);  				// determine color channel to write to
	z = ( z * 4.0f ) - float( channel );		// determine sample position from 1D texture
	
	// bit mask lookup determines bit value
	vec4 bit_value = texture( bitMask, z );
	
	if (sliceMapTarget == 0)
	{
		slice0_31 = bit_value;	
	}
	if (sliceMapTarget == 1)
	{
		slice32_63 = bit_value;
	}
	if (sliceMapTarget == 2)
	{
		slice64_93 = bit_value;
	}
	if (sliceMapTarget == 3)
	{
		slice94_127 = bit_value;
	}
}