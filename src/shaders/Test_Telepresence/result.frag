#version 430

uniform sampler2D m_resultTex;

 out vec4 fragColor;

 void main() {
   //fragColor = texture(m_resultTex, gl_FragCoord.xy);
	  fragColor = texelFetch(m_resultTex, ivec2(gl_FragCoord.xy),0);
	//  if( fragColor.y >= 0.2){
	//				 fragColor = vec4(1,0,0,1);
	//			}
}
