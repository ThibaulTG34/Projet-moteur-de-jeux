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

bool cube::Collision(vec3 pos1, vec3 size1, vec3 pos2, vec3 size2)
{
    // vec3 posMin1 = pos1 - vec3(size1/2.0f);
    // vec3 posMax1 = pos1 + vec3(size1/2.0f);
    // vec3 posMin2 = pos2 - vec3(size2/2.0f);
    // vec3 posMax2 = pos2 + vec3(size2/2.0f);

    // bool cond1 = posMin1.x < posMax2.x && posMax1.x > posMin1.x;
    // bool cond2 = posMin1.y < posMax2.y && posMax1.y > posMin1.y;
    // bool cond3 = posMin1.z < posMax2.z && posMax1.z > posMin1.z;

    // return cond1 && cond2 && cond3;

    // float max_distance_squared = (size1 + size2) * (size1 + size2);
    // vec3 posMax1 = pos1 + vec3(size1/2.0f);
    // vec3 posMax2 = pos2 + vec3(size2/2.0f);

    // float distance_squared = glm::length(posMax1 - posMax2);

    // return distance_squared < max_distance_squared;

    // Calculate the distances between each pair of opposite faces
    float dx = abs(pos2.x - pos1.x) - (size1.x + size2.x) / 2;
    float dy = abs(pos2.y - pos1.y) - (size1.y + size2.y) / 2;
    float dz = abs(pos2.z - pos1.z) - (size1.z + size2.z) / 2;

    // If any distance is negative, the cubes are colliding
    if (dx < 0 && dy < 0 && dz < 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
