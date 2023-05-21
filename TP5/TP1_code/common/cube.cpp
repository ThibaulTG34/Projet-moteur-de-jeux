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

#include "cube.h"

cube::cube()
{
}

cube::cube(int size, float scale_x, float scale_y, float scale_z, int mod)
{
    this->sommets = this->Vertices(size, scale_x, scale_y, scale_z);
    this->indices = {
        1, 0, 3,
        0, 2, 3,
        0, 4, 2,
        4, 6, 2,
        6, 2, 4,
        4, 5, 6,
        5, 7, 6,
        5, 1, 7,
        1, 3, 7,
        6, 7, 2,
        7, 3, 2,
        0, 1, 5,
        0, 4, 5};
    this->mode = mod;
}

vector<vec3> cube::Vertices(int size, float scale_x, float scale_y, float scale_z)
{
    vector<vec3> indexed_vertices;
    for (int i = 0; i < 8; i++)
    {
        float x = size * scale_x * (i & 1);
        float y = size * scale_y * ((i >> 1) & 1);
        float z = size * scale_z * (i >> 2);

        indexed_vertices.push_back(glm::vec3(x, y, z));
    }
    return indexed_vertices;
}

vec3 cube::FindMinPoint()
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

vec3 cube::FindMaxPoint()
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

bool cube::Collision(vec3 pos1, vec3 size1, vec3 pos2, vec3 size2)
{
    // // test 1
    // vec3 posMin1 = pos1 - vec3(size1/2.0f);
    // vec3 posMax1 = pos1 + vec3(size1/2.0f);
    // vec3 posMin2 = pos2 - vec3(size2/2.0f);
    // vec3 posMax2 = pos2 + vec3(size2/2.0f);

    // bool cond1 = posMin1.x < posMax2.x && posMax1.x > posMin1.x;
    // bool cond2 = posMin1.y < posMax2.y && posMax1.y > posMin1.y;
    // bool cond3 = posMin1.z < posMax2.z && posMax1.z > posMin1.z;
    // return cond1 && cond2 && cond3;

    // test 2
    // float max_distance_squared = (size1 + size2) * (size1 + size2);
    // vec3 posMax1 = pos1 + vec3(size1/2.0f);
    // vec3 posMax2 = pos2 + vec3(size2/2.0f);
    // float distance_squared = glm::length(posMax1 - posMax2);
    // return distance_squared < max_distance_squared;

    // test 3
    bool condX = pos2.x + size1.x >= pos2.x && pos2.x + size2.x >= pos1.x;
    bool condY = pos1.y + size1.x >= pos2.y && pos2.y + size2.x >= pos1.y;
    bool condZ = pos1.z + size1.x >= pos2.z && pos2.z + size2.x >= pos1.z;
    return condX && condY && condZ;

    // test 4
    // if ((pos2.x >= pos1.x + size1.x) || (pos2.x + size2.x <= pos1.x) || (pos2.z >= pos2.z + size1.z) || (pos2.z + size2.z <= pos1.z))
    // {
    //     return false;
    // }
    // else
    // {
    //     return true;
    // }

    // test 5
    if (abs(pos1.x - pos2.x) > (size1.x + size2.x))
        return false;

    if (abs(pos1.y - pos2.y) > (size1.y + size2.y))
        return false;

    if (abs(pos1.z - pos2.z) > (size1.z + size2.z))
        return false;

    return true;

    // float left = pos2.x - (pos1.x + size1.x);
    // float top = (pos2.y + size2.y) - pos1.y;
    // float right = (pos2.x + size2.x) - pos1.x;
    // float bottom = pos2.y - (pos1.y + size1.y);

    // return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

bool cube::CollisionDetection(vec3 max1, vec3 min1, vec3 max2, vec3 min2)
{
    if (max1.x < min2.x || max2.x < min1.x)
        return false;

    if (max1.y < min2.y || max2.y < min1.y)
        return false;

    if (max1.z < min2.z || max2.z < min1.z)
        return false;

    return true;
}
