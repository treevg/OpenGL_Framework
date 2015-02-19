/*
 * Objectloader.cpp
 *
 *  Created on: 04.01.2015
 *      Author: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
 *		Edited by: Moe
 */

#include "Objectloader.h"
Objectloader::Objectloader(){

}

bool Objectloader::loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals,
	bool hasUvs
){
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::vector<float> temp_vert_to_array;
	std::vector<float> temp_norm_to_array;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
	    printf("Impossible to open the file !\n Check, if path is correct");
	    return false;
	}

	while( 1 ){

	    char lineHeader[128];
	    // read the first word of the line
	    int res = fscanf(file, "%s", lineHeader);
	    if (res == EOF)
	        break; // EOF = End Of File. Quit the loop.

	    // else : parse lineHeader
	    if ( strcmp( lineHeader, "v" ) == 0 ){
	        glm::vec3 vertex;
	        fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
	        temp_vertices.push_back(vertex);
	    }else if ( strcmp( lineHeader, "vt" ) == 0 ){
	        glm::vec2 uv;
	        fscanf(file, "%f %f\n", &uv.x, &uv.y );
	        temp_uvs.push_back(uv);
	    }else if ( strcmp( lineHeader, "vn" ) == 0 ){
	        glm::vec3 normal;
	        fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
	        temp_normals.push_back(normal);
	    }else if ( strcmp( lineHeader, "f" ) == 0 ){
	       // std::string vertex1, vertex2, vertex3;
	        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

	        int matches;
	        if(hasUvs==true){
	        	 matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
	        }
	        else {
	        	 matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
	        }

	        if (matches == 9){
		        vertexIndices.push_back(vertexIndex[0]);
		        vertexIndices.push_back(vertexIndex[1]);
		        vertexIndices.push_back(vertexIndex[2]);
		        uvIndices    .push_back(uvIndex[0]);
		        uvIndices    .push_back(uvIndex[1]);
		        uvIndices    .push_back(uvIndex[2]);
		        normalIndices.push_back(normalIndex[0]);
		        normalIndices.push_back(normalIndex[1]);
		        normalIndices.push_back(normalIndex[2]);
	        }
	        else if(matches == 6){
	            vertexIndices.push_back(vertexIndex[0]);
	    		vertexIndices.push_back(vertexIndex[1]);
	    		vertexIndices.push_back(vertexIndex[2]);
//	    		uvIndices    .push_back(uvIndex[0]);
//	    		uvIndices    .push_back(uvIndex[1]);
//	    		uvIndices    .push_back(uvIndex[2]);
	    		normalIndices.push_back(normalIndex[0]);
	    		normalIndices.push_back(normalIndex[1]);
	    		normalIndices.push_back(normalIndex[2]);
	        }
	        else {

	        	printf("File can't be read by our simple parser. Try exporting with other options\n"
	        			"Supported options in Blender: Apply Modifiers, Include:Normals&UVs, Triangulate faces, Objects as OBJ objects \n");
	        	return false;
	        }
	    }

	}
	    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
	    	unsigned int vertexIndex = vertexIndices[i];
	    	glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
	    	glm::vec3 normal = temp_normals[vertexIndex-1 ];
	    	out_vertices.push_back(vertex);
	    	out_normals.push_back(normal);

	    	temp_vert_to_array.push_back(vertex.x);
	    	temp_vert_to_array.push_back(vertex.y);
	    	temp_vert_to_array.push_back(vertex.z);
	    	temp_vert_to_array.push_back(0.0);

	    	temp_norm_to_array.push_back(normal.x);
	    	temp_norm_to_array.push_back(normal.y);
	    	temp_norm_to_array.push_back(normal.z);
	    	temp_norm_to_array.push_back(0);
	    	//cout <<"LOOK HERE: "<< to_string(vertex) << endl;
	    }

	    std::copy(temp_vert_to_array.begin(),temp_vert_to_array.end(),meshData);
	    std::copy(temp_norm_to_array.begin(),temp_norm_to_array.end(),meshNormal);
}


