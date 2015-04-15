#version 430

uniform vec4 color;
uniform float blendColor;
uniform sampler2D tex;

uniform vec2 resolution;
uniform mat4 model;

uniform mat4 view;
uniform mat4 projection;

uniform mat4 projectorView;
uniform mat4 projectorProjection;

in vec4 passWorldPosition;
in vec4 passPosition;
in vec2 passUVCoord;
in vec3 passWorldNormal;
in vec3 passNormal;

out vec4 fragColor;

void main() {
	vec2 fragCoord = vec2(gl_FragCoord.xy / resolution) * 2 - 1;
	// vec3 viewDir = normalize(inverse(mat3(projection) * mat3(view)) * vec3(fragCoord, 1));
	// vec3 dir = reflect(viewDir, inverse(mat3(view)) * passNormal.xyz);
	// vec4 environment = texture(tex, vec2(atan(dir.x, dir.z) / 6.2832 + 0.5, acos(dir.y) / 3.1416));

	vec3 lightVector = -normalize((inverse(projectorProjection * projectorView) * vec4(0,0,0,1)).xyz);
	float lightStrength = max(dot(passWorldNormal, lightVector), 0.0);


	vec4 projPos = projectorProjection * projectorView * passWorldPosition;
	vec3 projUV = (projPos.xyz / projPos.a + 1.0) / 2.0;
    fragColor = texture(tex, projUV.xy) * lightStrength;
}