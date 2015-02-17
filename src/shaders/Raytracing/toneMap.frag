#version 430

uniform sampler2D depth;
uniform float minRange;
uniform float maxRange;

in vec4 passPosition;

out vec4 fragColor;


float LinearizeDepth(vec2 uv)
{
  float n = 1.0; // camera z near
  float f = 10.0; // camera z far
  float z = texture2D(depth, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));	
}

vec3 lin() {
	vec3 c = texture(depth, (passPosition.xy - 1) * 0.5).xyz;
	c = (c - minRange) / (maxRange - minRange);
	
	return c;  
}

void main() {
	fragColor = vec4(lin(),1);

}