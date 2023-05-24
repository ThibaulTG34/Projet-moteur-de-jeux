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

BBOX::BBOX() {}

BBOX::BBOX(vector<vec3> vertices, vec3 epsilon)
{
    vec3 bbMax = this->computeBbMax(epsilon, vertices);
    vec3 bbMin = this->computeBbMin(epsilon, vertices);
    this->BBmax = bbMax;
    this->BBmin = bbMin;
    /* this->sommets =  */ this->computeBBOXVertices(bbMin, bbMax);
    /* this->indices =  */ this->computeBBOXIndices();
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

void BBOX::computeBBOXVertices(vec3 bbMin, vec3 bbMax)
{
    // vector<vec3> bbox;

    // Sommet devant haut gauche
    vec3 devant_haut_gauche = vec3(bbMin.x, bbMax.y, bbMax.z);
    this->sommets.push_back(devant_haut_gauche);

    // Sommet devant haut droite
    vec3 devant_haut_droite = bbMax;
    this->sommets.push_back(devant_haut_droite);

    // Sommet devant bas gauche
    vec3 devant_bas_gauche = vec3(bbMin.x, bbMin.y, bbMax.z);
    this->sommets.push_back(devant_bas_gauche);

    // Sommet devant bas droite
    vec3 devant_bas_droite = vec3(bbMax.x, bbMin.y, bbMax.z);
    this->sommets.push_back(devant_bas_droite);

    // Sommet derriere bas droite
    vec3 derriere_bas_droite = vec3(bbMax.x, bbMin.y, bbMin.z);
    this->sommets.push_back(derriere_bas_droite);

    // Sommet derriere haut droite
    vec3 derriere_haut_droite = vec3(bbMax.x, bbMax.y, bbMin.z);
    this->sommets.push_back(derriere_haut_droite);

    // Sommet derriere bas gauche
    vec3 derriere_bas_gauche = bbMin;
    this->sommets.push_back(derriere_bas_gauche);

    // Sommet derriere haut gauche
    vec3 derriere_haut_gauche = vec3(bbMin.x, bbMax.y, bbMin.z);
    this->sommets.push_back(derriere_haut_gauche);

    // return bbox;
}

void BBOX::computeBBOXIndices()
{
    vector<unsigned short> indices;
    this->indices =
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

    // return indices;
}

void BBOX::drawEntity(uint programID)
{
    for (unsigned int i = 0; i < this->children.size(); i++)
    {
        children[i]->drawEntity(programID);
    }
}

vec3 BBOX::getBbmin()
{
    return this->BBmin;
}

vec3 BBOX::getBbmax()
{
    return this->BBmax;
}

bool BBOX::CollisionBoxBox(vec3 minPoint1, vec3 maxPoint1, vec3 minPoint2, vec3 maxPoint2, mat4 transformMatrix1, mat4 transformMatrix2)
{
    glm::vec3 transformedMinPointLeg = glm::vec3(transformMatrix1 * glm::vec4(minPoint1, 1.0f));
    glm::vec3 transformedMaxPointLeg = glm::vec3(transformMatrix1 * glm::vec4(maxPoint1, 1.0f));
    glm::vec3 transformedMinPointCube = glm::vec3(transformMatrix2 * glm::vec4(minPoint2, 1.0f));
    glm::vec3 transformedMaxPointCube = glm::vec3(transformMatrix2 * glm::vec4(maxPoint2, 1.0f));

    if (transformedMinPointLeg.x > transformedMaxPointCube.x || transformedMaxPointLeg.x < transformedMinPointCube.x)
    {
        return false;
    }
    if (transformedMinPointLeg.y > transformedMaxPointCube.y || transformedMaxPointLeg.y < transformedMinPointCube.y)
    {
        return false;
    }
    if (transformedMinPointLeg.z > transformedMaxPointCube.z || transformedMaxPointLeg.z < transformedMinPointCube.z)
    {
        return false;
    }

    return true;
}

bool BBOX::CollisionBoxSphere(vec3 sphereCenter, float radius, vec3 point, vec3 updatePos, mat4 transformMatrix1, mat4 transformMatrix2)
{
    glm::vec3 transformedSphereCenter = glm::vec3(transformMatrix1 * glm::vec4(sphereCenter, 1.0f));
    glm::vec3 transformedMaxPoint = glm::vec3(transformMatrix2 * glm::vec4(point + updatePos, 1.0f));
    glm::vec3 transformedMinPoint = glm::vec3(transformMatrix2 * glm::vec4(point + updatePos, 1.0f));

    glm::vec3 closestPoint = glm::clamp(transformedSphereCenter, transformedMinPoint, transformedMaxPoint);

    float distance1 = length(closestPoint - transformedSphereCenter);
    float distance2 = length(closestPoint - transformedSphereCenter);

    // cout << distance2 << endl;

    if (distance1 <= radius || distance2 <= radius)
    {
        return true;
    }

    // glm::vec3 transformedPoint = glm::vec3(transformMatrix2 * glm::vec4(point + updatePos, 1.0f));

    // vec3 spherePoint_vect = transformedPoint - transformedSphereCenter;
    // spherePoint_vect = normalize(spherePoint_vect);

    // spherePoint_vect *= radius;

    // vec3 worldPoint = transformedSphereCenter + spherePoint_vect;

    // vec3 closestPointSphere_vect = transformedSphereCenter - worldPoint;

    // float dist_squared = length(closestPointSphere_vect);

    // return dist_squared < radius * radius;

}

void BBOX::calculerSphereEnglobante(const std::vector<vec3> &points, vec3 &centre, double &rayon)
{
    // Calculer le centre de la sphère (moyenne des coordonnées des points)
    centre = {0.0, 0.0, 0.0};
    for (const auto &point : points)
    {
        centre.x += point.x;
        centre.y += point.y;
        centre.z += point.z;
    }
    centre.x /= points.size();
    centre.y /= points.size();
    centre.z /= points.size();

    cout << centre.x << ", " << centre.y << ", " << centre.z << endl;
}