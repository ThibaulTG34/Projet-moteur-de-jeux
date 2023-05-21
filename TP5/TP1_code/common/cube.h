#ifndef CUBE_H
#define CUBE_H

#include <string>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#include "EntityRoot.h"

class cube : public EntityRoot
{
public:
    cube();

    cube(int size, float scale_x, float scale_y, float scale_z, int mod);
    vector<vec3> Vertices(int size, float scale_x, float scale_y, float scale_z);
    vec3 FindMinPoint();
    vec3 FindMaxPoint();
    bool Collision(vec3 pos1, vec3 size1, vec3 pos2, vec3 size2);
    bool CollisionDetection(vec3 max1, vec3 min1, vec3 max2, vec3 min2);
};

#endif