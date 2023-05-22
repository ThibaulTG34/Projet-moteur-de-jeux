// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

#include "objloader.hpp"
#include "Entity.h"
#include "texture.hpp"
#include "vboindexer.hpp"

// Constructeur par defaut
Entity::Entity() {}

// Constructeur 1
Entity::Entity(const string path, const string textpath, int loading, int mode)
{
	if (loading == 0)
	{
		loadOFF(path, this->sommets, this->indices, this->triangles);
		this->texture = loadTexture2DFromFilePath(textpath);
		this->uvs = this->compute_uv(this->sommets);
	}
	else if(loading == 1)
	{
		this->texture = loadTexture2DFromFilePath(textpath);
		loadAssImp(path.c_str(), this->indices, this->sommets);
		this->uvs = this->compute_uv(this->sommets);
	}
	else
	{
		this->texture = loadTexture2DFromFilePath(textpath);
		loadOBJ(path.c_str(), this->sommets, this->uvs, this->normales);
		indexVBO(this->sommets, this->uvs, this->normales, this->indices, this->sommets, this->uvs, this->normales);
		this->uvs = this->compute_uv(this->sommets);
	}
}

vector<vec2> Entity::compute_uv(vector<vec3> vertices)
{
	vector<vec2> UVs;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		float u = 0.5 + atan(vertices[i].z, vertices[i].x) / (2 * M_PI);
		float v = 0.5 + asin(vertices[i].y) / (M_PI);
		UVs.push_back(vec2(u, v));
	}

	return UVs;
}

vector<vector<vec3>> Entity::compute_triangle(vector<vec3> vertices, vector<unsigned short> indices)
{
	vector<vector<vec3>> triangles;
	vector<vec3> triangle;

	for(unsigned int i=0; i<indices.size(); i+=3)
	{
		triangle.push_back(vertices[i+0]);
		triangle.push_back(vertices[i+1]);
		triangle.push_back(vertices[i+2]);

		triangles.push_back(triangle);
	}

	return triangles;

}

vector<vec3> Entity::compute_normal(vector<vec3> vertices, vector<unsigned short> indices)
{
	vector<vector<vec3>> triangles = this->compute_triangle(vertices, indices);
	vector<vec3> normals;

	for(unsigned int i=0; i<triangles.size(); i++)
	{
		vec3 sommet_1 = triangles[i][0];
		vec3 sommet_2 = triangles[i][1];
		vec3 sommet_3 = triangles[i][2];

		float side_1_x = sommet_1.x - sommet_2.x;
		float side_1_y = sommet_1.y - sommet_2.y;
		float side_1_z = sommet_1.z - sommet_2.z;

		vec3 side_1 = vec3(side_1_x, side_1_y, side_1_z);

		float side_2_x = sommet_2.x - sommet_3.x;
		float side_2_y = sommet_2.y - sommet_3.y;
		float side_2_z = sommet_2.z - sommet_3.z;

		vec3 side_2 = vec3(side_2_x, side_2_y, side_2_z);

		vec3 normal = cross(side_1, side_2);

		normals.push_back(normal);
	}

	return normals;
}

vec3 Entity::FindMinPoint()
{
    glm::vec3 minPoint = this->sommets[0];

    for (size_t i = 0; i < this->sommets.size(); ++i)
    {
        vec3 &vertex = this->sommets[i];
        minPoint.x = std::min(minPoint.x, vertex.x);
        minPoint.y = std::min(minPoint.y, vertex.y);
        minPoint.z = std::min(minPoint.z, vertex.z);
    }

    return minPoint;
}

vec3 Entity::FindMaxPoint()
{
    glm::vec3 minPoint = this->sommets[0];

    for (size_t i = 0; i < this->sommets.size(); ++i)
    {
        vec3 &vertex = this->sommets[i];
        minPoint.x = std::max(minPoint.x, vertex.x);
        minPoint.y = std::max(minPoint.y, vertex.y);
        minPoint.z = std::max(minPoint.z, vertex.z);
    }

    return minPoint;
}