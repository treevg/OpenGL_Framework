/*
 * Objectloader.h
 *
 *  Created on: 04.01.2015
 *      Author: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
 *		Edited by: Moe
 */

#ifndef LIBRARIES_OBJECTLOADER_OBJECTLOADER_H_
#define LIBRARIES_OBJECTLOADER_OBJECTLOADER_H_


#include "iostream"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <vector>


class Objectloader {
public:
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	float meshData[100000];
	float meshNormal[100000];

	Objectloader();

	bool loadOBJ(
	    const char * path,
	    std::vector < glm::vec3 > & out_vertices,
	    std::vector < glm::vec2 > & out_uvs,
	    std::vector < glm::vec3 > & out_normals,
		bool hasUvs
	);


};


#endif /* LIBRARIES_OBJECTLOADER_OBJECTLOADER_H_ */
