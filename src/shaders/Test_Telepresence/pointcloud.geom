#version 400
//#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(points, max_vertices = 1) out;

in VertexData
{
    vec4 vertexPosition;
	vec3 vertexColor;
} vertex[];

out float depth;
out vec3 fragmentColor;

void main()
{   
	if(gl_in[0].gl_Position.xyz != vec3(0))
	{
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		//gl_Position = gl_in[0].gl_Position+vec4(.001,0,0,0);
		//EmitVertex();
		//gl_Position = gl_in[0].gl_Position+vec4(0, .001, 0, 0);
		//EmitVertex();

		depth = vertex[0].vertexPosition.z;
		fragmentColor = vertex[0].vertexColor;

		EndPrimitive();
	}
}