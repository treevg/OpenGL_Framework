#version 430 

// a fragment shader which sets the corresponding 
// pixel bit voxel in the slice map to 1

in float clipDepth; // relative position between near and far plane

//uniform sampler2D bitMask;
layout( rgba32ui, binding = 0 ) uniform readonly uimage1D bitMask;

//uniform int numSlicemaps;	// number of slicemaps to accumulate (max 4)

// out : layout positions for multiple render targets
layout(location = 0) out uvec4 slice0_127;

// (optional) use multiple render targets
//layout(location = 1) out uvec4 slice128_255;
//layout(location = 2) out uvec4 slice256_383;
//layout(location = 3) out uvec4 slice384_511;
 
void main() { 
	float z = clipDepth; // [0..1]
	
//	// determine slicemap to write to
//	int sliceMapTarget = int ( numSlicemaps * z );
//	
//	// map from z to [0..1] in slice map target
//	z = ( z * float( numSlicemaps ) ) - float( sliceMapTarget ); 
//	
	// bit mask lookup determines bit value
	uvec4 bit_value = imageLoad( bitMask, int( z * 128.0 )  );
	
//	if (sliceMapTarget == 0)
//	{
		slice0_127 = bit_value;
//	}
//	if (sliceMapTarget == 1)
//	{
//		slice128_255 = bit_value;
//	}
//	if (sliceMapTarget == 2)
//	{
//		slice256_383 = bit_value;
//	}
//	if (sliceMapTarget == 3)
//	{
//		slice384_511 = bit_value;
//	}
	
}