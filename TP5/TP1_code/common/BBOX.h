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

    void computeBBOXVertices(vec3 bbMin, vec3 bbMax);

    void computeBBOXIndices();

    bool Collision(vec3 pos1, vec3 size1, vec3 pos2, vec3 size2);
};

#endif