#version 410

in vec4 vertex_position;
 in vec3 vertex_normal;
 in vec2 texture_Coordinates;


uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;
//uniform vec3 uniformLightPosition;

out vec4 passPosition;
out vec2 passTextureCoordinate;
out vec3 passNormal;

//out vec3 passLightDirection;



void main() {

	passNormal = vertex_normal;
        passPosition =  uniformModel* vertex_position;
	gl_Position = uniformProjection * uniformView * uniformModel* vertex_position;



    //    vec3 passLightDirection = (uniformView * vec4(uniformLightPosition,1)).xyz;
   //     vec3 inverse_normal = normalize(transpose(inverse(mat3(model))) * vertex_normal)
        passNormal = (uniformView * uniformModel * vec4(vertex_normal,0)).xyz; 
  
}
