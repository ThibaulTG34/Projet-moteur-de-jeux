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

#include "Sphere.h"
#include "texture.hpp"

Sphere::Sphere()
{
}

Sphere::Sphere(const string textpath, int slices, int stacks, float radius)
{
    this->texture = loadTexture2DFromFilePath(textpath);
    this->sphere(radius, slices, stacks, this->sommets, this->indices);
    this->uvs = this->compute_uv(this->sommets);
}

Sphere::Sphere(vector<vec3> vertices, int slices, int stacks)
{
    // float rayon = 0.f;
    vec3 centre = {0.0, 0.0, 0.0};
    for (const auto &point : vertices)
    {
        centre.x += point.x;
        centre.y += point.y;
        centre.z += point.z;
    }
    centre.x /= vertices.size();
    centre.y /= vertices.size();
    centre.z /= vertices.size();

    for (const auto &point : vertices)
    {
        double distance = sqrt(pow(point.x - centre.x, 2) +
                               pow(point.y - centre.y, 2) +
                               pow(point.z - centre.z, 2));
        if (distance > this->rayon)
        {
            this->rayon = distance;
        }
    }

    this->sphere(this->rayon, slices, stacks, this->sommets, this->indices);
}

void Sphere::sphere(float radius, int nb_slices, int nb_stacks, vector<vec3> &vertices, vector<unsigned short> &indices)
{
    for (int i = 0; i <= nb_slices; i++)
    {
        float theta1 = i * 2.0 * M_PI / nb_slices;
        float theta2 = (i + 1) * 2.0 * M_PI / nb_slices;

        for (int j = 0; j <= nb_stacks; j++)
        {
            float phi = j * M_PI / nb_stacks;
            float x1 = radius * sin(phi) * cos(theta1);
            float y1 = radius * sin(phi) * sin(theta1);
            float z1 = radius * cos(phi);

            float x2 = radius * sin(phi) * cos(theta2);
            float y2 = radius * sin(phi) * sin(theta2);
            float z2 = radius * cos(phi);

            if (i != nb_slices && j != nb_stacks)
            {
                indices.push_back(i * (nb_stacks + 1) + j);
                indices.push_back((i + 1) * (nb_stacks + 1) + j);
                indices.push_back((i + 1) * (nb_stacks + 1) + (j + 1));

                indices.push_back(i * (nb_stacks + 1) + j);
                indices.push_back((i + 1) * (nb_stacks + 1) + (j + 1));
                indices.push_back(i * (nb_stacks + 1) + (j + 1));
            }

            vertices.push_back(glm::vec3(x1, y1, z1));
        }
    }
}

float Sphere::getRayon()
{
    return rayon;
}

vector<vec2> Sphere::compute_uv(vector<vec3> vertices)
{
    vector<vec2> UVs;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        float u = 0.5 + atan(vertices[i].z, vertices[i].x) / (2 * M_PI);
        float v = 0.5 + asin(vertices[i].y) / (M_PI);
        UVs.push_back(vec2(u, v));
    }

    return UVs;
}

void Sphere::DifferentModel()
{
    this->resolutions_vertices.resize(5);
    this->resolutions_indices.resize(5);
    this->resolutions_uvs.resize(5);

    this->sphere(0.4, 50, 50, resolutions_vertices[0], resolutions_indices[0]);
    resolutions_uvs[0] = this->compute_uv(resolutions_vertices[0]);
    this->sphere(0.4, 40, 40, resolutions_vertices[1], resolutions_indices[1]);
    resolutions_uvs[1] = this->compute_uv(resolutions_vertices[1]);
    this->sphere(0.4, 30, 30, resolutions_vertices[2], resolutions_indices[2]);
    resolutions_uvs[2] = this->compute_uv(resolutions_vertices[2]);
    this->sphere(0.4, 20, 20, resolutions_vertices[3], resolutions_indices[3]);
    resolutions_uvs[3] = this->compute_uv(resolutions_vertices[3]);
    this->sphere(0.4, 10, 10, resolutions_vertices[4], resolutions_indices[4]);
    resolutions_uvs[4] = this->compute_uv(resolutions_vertices[4]);
}

void Sphere::updateBuffers(vector<vec3> vertices, vector<unsigned short> indices, vector<vec2> uv)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(vec2), &uv[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void Sphere::drawEntity(uint programID)
{
    for (unsigned int i = 0; i < this->children.size(); i++)
    {
        children[i]->drawEntity(programID);
    }
}