#version 430

layout(location = 0) in vec4 positionAttribute;

uniform sampler2D worldPosition;
uniform vec2 resolution;
uniform mat4 view;
uniform mat4 projection;

out vec4 passWorldPosition;
out vec2 passUVCoord;

void main() {
	vec4 worldPos = texture(worldPosition, positionAttribute.xy);
	gl_Position = projection * view * worldPos;

	passWorldPosition = worldPos;
	passUVCoord = positionAttribute.xy;
	// vec2 uv = vec2(gl_FragCoord.xy / resolution);
	// vec3 viewDir_rev = vec3(sin((uv.x - 0.5) * 6.2832) * sin(uv.y * 3.1416), -cos(uv.y * 3.1416), cos((uv.x - 0.5) * 6.2832) * sin(uv.y * 3.1416));
	// vec3 viewCoord_rev = mat3(projection) * mat3(view) * (viewDir_rev);
	// vec2 fragCoord_rev = viewCoord_rev.xy / viewCoord_rev.z;
	//
	// if(fragCoord_rev.x > 1
	// 	|| fragCoord_rev.x < -1
	// 	|| fragCoord_rev.y > 1
	// 	|| fragCoord_rev.y < -1
	// 	||  viewCoord_rev.z < 0) discard;
	//
	// fragColor = texture(tex, (fragCoord_rev + 1) / 2);
}
