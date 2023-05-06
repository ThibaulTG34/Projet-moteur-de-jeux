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

	Entity(const string path, const string textpath, int loading);

	vector<vec2> compute_uv(vector<vec3> vertices);

};

#endif