#version 430

in float depth;
in vec3 fragmentColor;

out vec4 fragColor;

void main() {

	fragColor = vec4(fragmentColor, depth);
}


//float m_bmin = 1.0;
//float m_bmax = 8.0;
//float m_bavg = 4.5;
//float computeWeight( float depth )
//{
//	clamp(depth, 0.0, 8.0);
//	float disparity = m_bmax - depth;
//	float gamma = 3 / 2 * m_bavg + m_bmin;
//	float sigma = 4 / 3 * m_bavg + m_bmin;
//	float delta = 1 / 2 * m_bavg + m_bmin;

//	float exponent = -abs( ( m_bmax + delta) / depth);
//	float weight = gamma / sigma * (1-exp(exponent)) + 0.1;

//	return weight;
//}