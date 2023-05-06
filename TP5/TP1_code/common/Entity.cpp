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

// Constructeur par defaut
Entity::Entity() {}

// Constructeur 1
Entity::Entity(const string path, const string textpath, int loading)
{
	if (loading == 0)
	{
		loadOFF(path, this->sommets, this->indices, this->triangles);
		this->texture = loadTexture2DFromFilePath(textpath);
		this->uvs = this->compute_uv(this->sommets);
	}
	else
	{
		this->texture = loadTexture2DFromFilePath(textpath);
		loadAssImp(path.c_str(), this->indices, this->sommets);
		// this->uvs = this->compute_uv(this->sommets);
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