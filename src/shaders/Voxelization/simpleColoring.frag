#version 430

uniform bool hasTexture;
uniform sampler2D texture1;
uniform float textureTransparency;

uniform float red;
uniform float green;
uniform float blue;
uniform float alpha;

in vec2 passUV;

out vec4 fragmentColor;

void main() {
	vec4 texColor = vec4( 0, 0, 0, 0 );
	
	if ( hasTexture )
	{
		texColor = texture(texture1, passUV);
	}
	
	vec4 color = vec4( red, green, blue, alpha );
	
    fragmentColor = vec4 ( 
    		max ( 0.0, min( 1.0 ,         ( texColor.a - textureTransparency ) ) )   * texColor.rgb 
    	  + max ( 0.0, min( 1.0 , ( 1.0 - ( texColor.a - textureTransparency ) ) ) ) * color.rgb   , alpha + texColor.a );
}