// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BBOX.h"

using namespace glm;

BBOX::BBOX(){}

BBOX::BBOX(vector<vec3> vertices, vec3 epsilon){
    vec3 bbMax = this->computeBbMax(epsilon, vertices);
    vec3 bbMin = this->computeBbMin(epsilon, vertices);
    this->sommets = this->computeBBOXVertices(bbMin, bbMax);
    this->indices = this->computeBBOXIndices();

}

// Calcule BBmin
vec3 BBOX::computeBbMin(vec3 epsilon, vector<vec3> vertices)
{
    float min_x, min_y, min_z;
    min_x = min_y = min_z = FLT_MAX;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i][0] < min_x)
        {
            min_x = vertices[i][0];
        }
        if (vertices[i][1] < min_y)
        {
            min_y = vertices[i][1];
        }
        if (vertices[i][2] < min_z)
        {
            min_z = vertices[i][2];
        }
    }

        return vec3(min_x, min_y, min_z) + epsilon;
}

// Calcule BBmax
vec3 BBOX::computeBbMax(vec3 epsilon, vector<vec3> vertices)
{
    float max_x, max_y, max_z;
    max_x = max_y = max_z = FLT_MIN;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i][0] > max_x)
        {
            max_x = vertices[i][0];
        }
        if (vertices[i][1] > max_y)
        {
            max_y = vertices[i][1];
        }
        if (vertices[i][2] > max_z)
        {
            max_z = vertices[i][2];
        }
    }

    return vec3(max_x, max_y, max_z) + epsilon;
}

vector<vec3> BBOX::computeBBOXVertices(vec3 bbMin, vec3 bbMax)
{
    vector<vec3> bbox;

    //Sommet devant haut gauche
    vec3 devant_haut_gauche = vec3(bbMin.x, bbMax.y, bbMax.z);
    bbox.push_back(devant_haut_gauche);

    //Sommet devant haut droite
    vec3 devant_haut_droite = bbMax;
    bbox.push_back(devant_haut_droite);

    //Sommet devant bas gauche
    vec3 devant_bas_gauche = vec3(bbMin.x, bbMin.y, bbMax.z);
    bbox.push_back(devant_bas_gauche);

    //Sommet devant bas droite
    vec3 devant_bas_droite = vec3(bbMax.x, bbMin.y, bbMax.z);
    bbox.push_back(devant_bas_droite);

    //Sommet derriere bas droite
    vec3 derriere_bas_droite = vec3(bbMax.x, bbMin.y, bbMin.z);
    bbox.push_back(derriere_bas_droite);

    //Sommet derriere haut droite
    vec3 derriere_haut_droite = vec3(bbMax.x, bbMax.y, bbMin.z);
    bbox.push_back(derriere_haut_droite);

    //Sommet derriere bas gauche
    vec3 derriere_bas_gauche = bbMin;
    bbox.push_back(derriere_bas_gauche);

    //Sommet derriere haut gauche
    vec3 derriere_haut_gauche = vec3(bbMin.x, bbMax.y, bbMin.z);
    bbox.push_back(derriere_haut_gauche);

    return bbox;
}

vector<unsigned short> BBOX::computeBBOXIndices(){
    vector<unsigned short> indices;
    indices = 
    {
        0, 1, 2,
        2, 1, 3,
        3, 2, 6,
        3, 6, 4,
        1, 5, 3,
        3, 5, 4,
        5, 7, 4,
        4, 7, 6,
        7, 0, 6,
        6, 0, 2,
        1, 0, 7,
        1, 7, 5

    };

    return indices;

}

bool BBOX::Collision(vec3 pos1, vec3 size1, vec3 pos2, vec3 size2)
{
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