#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#include "Transform.h"
#include "EntityRoot.h"


class Entity:public EntityRoot
{
public:

	Entity();

	Entity(const string path, const string textpath, int loading, int mode);

	vector<vec2> compute_uv(vector<vec3> vertices);

	vector<vector<vec3>> compute_triangle(vector<vec3> vertices, vector<unsigned short> indices);

	vector<vec3> compute_normal(vector<vec3> vertices, vector<unsigned short> indices);

	vec3 FindMinPoint();

	vec3 FindMaxPoint();

};

#endif