#version 430

uniform sampler2D m_resultTex;

 out vec4 fragColor;

 void main() {
	  fragColor = texelFetch(m_resultTex, ivec2(gl_FragCoord.xy),0);
		fragColor.a = 1;
}
