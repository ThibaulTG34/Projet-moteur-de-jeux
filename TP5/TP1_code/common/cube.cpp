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

cube::cube(){}

cube::cube(int size, float scale_x, float scale_y, float scale_z){
    this->sommets = this->Vertices(size, scale_x, scale_y, scale_z);
    this->indices =  {
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

}

vector<vec3> cube::Vertices(int size, float scale_x, float scale_y, float scale_z){
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

