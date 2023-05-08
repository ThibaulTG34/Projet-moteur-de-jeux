#ifndef BBOX_H
#define BBOX_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "EntityRoot.h"

using namespace std;
using namespace glm;

class BBOX:public EntityRoot
{
public:

    BBOX();

    BBOX(vector<vec3> vertices, vec3 epsilon);

    vec3 computeBbMin(vec3 epsilon, vector<vec3> vertices);

    vec3 computeBbMax(vec3 epsilon, vector<vec3> vertices);

    vector<vec3> computeBBOXVertices(vec3 bbMin, vec3 bbMax);

    vector<unsigned short> computeBBOXIndices();
};

#endif