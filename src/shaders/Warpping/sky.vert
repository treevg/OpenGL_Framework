#version 430

uniform mat4 uniformView;

out vec3 passTextureCoord;
out vec4 passPosition;



void main() {
			vec3[4] verticies = vec3[4](vec3(-1.0,-1.0, 1.0),
							            vec3( 1.0,-1.0, 1.0),
							            vec3(-1.0, 1.0, 1.0),
							            vec3( 1.0, 1.0, 1.0));
	        passTextureCoord = mat3(uniformView) * verticies[gl_VertexID] ;
	        passPosition =  vec4(verticies[gl_VertexID], 1.0);
                gl_Position =vec4(verticies[gl_VertexID], 1.0);
            
   
}
