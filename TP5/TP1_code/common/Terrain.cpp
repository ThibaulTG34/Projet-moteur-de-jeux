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
#include <glm/gtc/matrix_transform.hpp>

#include "Terrain.h"
#include "texture.hpp"

using namespace glm;

Terrain::Terrain(int resolution, int taille, float offset, const string path, const string heightmap_path, int mode, uint programID)
{
    this->sommets = this->Vertices(resolution, taille, offset);
    this->indices = this->Indices(resolution, taille, offset);
    this->texture = loadTexture2DFromFilePath(path);
    this->uvs = this->compute_uv(this->sommets);
    this->mode = 1;
    this->heightmap = loadTexture2DFromFilePath(heightmap_path);
    this->triangles = this->Triangles(this->indices);
}

Terrain::Terrain(vector<vec3> sommets, vector<unsigned short> indices, float offset, const string path, const string heightmap_path, int mode, uint programID)
{
    for (size_t i = 0; i < sommets.size(); i++)
    {
        this->sommets.push_back(glm::vec3(sommets[i].x, sommets[i].y, sommets[i].z - offset));
    }

    for (size_t i = 0; i < indices.size(); i++)
    {
        this->indices.push_back(indices[i]);
    }

    this->texture = loadTexture2DFromFilePath(path);
    this->uvs = this->compute_uv(this->sommets);
    this->mode = 1;
    this->heightmap = loadTexture2DFromFilePath(heightmap_path);
    this->triangles = this->Triangles(this->indices);
}

float Terrain::FindMinZ()
{
    float min = this->sommets[0].z;
    for (int i = 0; i < this->sommets.size(); i++)
    {
        if (min > this->sommets[i].z)
        {
            min = this->sommets[i].z;
        }
    }

    return min;
}

float Terrain::FindMaxZ()
{
    float max = this->sommets[0].z;
    for (int i = 0; i < this->sommets.size(); i++)
    {
        if (max < this->sommets[i].z)
        {
            max = this->sommets[i].z;
        }
    }

    return max;
}

std::vector<glm::vec3> Terrain::Vertices(int resolution, int taille, float offset)
{
    std::vector<glm::vec3> list_vertices;

    for (unsigned int i = 0; i < resolution; i++)
    {
        for (unsigned int j = 0; j < resolution; j++)
        {
            glm::vec3 vertex = glm::vec3(((float)i * 1 / (float)(resolution - 1)) - offset, 0, ((float)j * 7 / (float)(resolution - 1)) - offset);
            list_vertices.push_back(vertex);
        }
    }

    return list_vertices;
}

std::vector<unsigned short> Terrain::Indices(int resolution, int taille, float offset)
{
    std::vector<unsigned short> list_indices;

    for (unsigned int i = 0; i < resolution - 1; i++)
    {
        for (unsigned int j = 0; j < resolution - 1; j++)
        {
            int i1 = i * resolution + j;
            list_indices.push_back(i1);
            int i2 = (i + 1) * resolution + j;
            list_indices.push_back(i2);
            int i3 = i * resolution + j + 1;
            list_indices.push_back(i3);

            int i4 = i * resolution + j + 1;
            list_indices.push_back(i4);
            int i5 = (i + 1) * resolution + j;
            list_indices.push_back(i5);
            int i6 = (i + 1) * resolution + j + 1;
            list_indices.push_back(i6);
        }
    }

    return list_indices;
}

std::vector<std::vector<unsigned short>> Terrain::Triangles(std::vector<unsigned short> indices)
{
    std::vector<std::vector<unsigned short>> triangles;
    for (unsigned int i = 0; i < indices.size(); i += 3)
    {
        std::vector<unsigned short> triangle;
        triangle.push_back(indices[i]);
        triangle.push_back(indices[i + 1]);
        triangle.push_back(indices[i + 2]);
        triangles.push_back(triangle);
    }

    return triangles;
}

std::vector<vec2> Terrain::compute_uv(vector<vec3> vertices)
{
    vector<vec2> UVs;
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        UVs.push_back(vec2(vertices[i].x, vertices[i].z));
    }
    return UVs;
}

void Terrain::InfinitePlane(float offset)
{
    for (size_t i = 0; i < this->sommets.size(); i++)
    {
        this->sommets[i].z -= offset;
    }
}