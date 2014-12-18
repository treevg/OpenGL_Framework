#version 430 

// a fragment shader which sets the corresponding 
// pixel bit voxel in the slice map to 1

in float clipDepth; // relative position between near and far plane

// layout( rgba32ui, binding = 0 ) uniform readonly uimage1D bitMask;

// out : layout positions for multiple render targets
layout( location = 0 ) out uvec4 slice0_127;
 
void main() { 
	float z = clipDepth; // [0..1]
	
	// bit mask lookup determines bit value
//	uvec4 bit_value = imageLoad( bitMask, int( z * 128.0 )  );
	
	// direct bit mask computation
	uvec4 bit_value = uvec4(0,0,0,0);	
	if ( z >= 0. && z < 1.0)
	{
		if (z < 0.25)
			bit_value.r = 0x10000000 >> uint( z * 128.0 );
		else if (z < 0.5)
			bit_value.g = 0x10000000 >> uint( (z - 0.25) * 128.0 );	
		else if (z < 0.75)
			bit_value.b = 0x10000000 >> uint( (z - 0.5) * 128.0 );
		else	
			bit_value.a = 0x10000000 >> uint( (z - 0.75) * 128.0 );
	}
	
	// write bitmask into framebuffer
	slice0_127 = bit_value;
}