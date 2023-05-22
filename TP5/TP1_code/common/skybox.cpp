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

#include "texture.hpp"
#include "skybox.h"

using namespace std;
using namespace glm;

skybox::skybox() {}

skybox::skybox(vector<string> faces, float size, int mode)
{
    this->sk = loadSkybox(faces);
    this->sommets = this->Vertices(size);
    this->indices = this->Indices();
    this->mode = mode;
}

vector<vec3> skybox::Vertices(float size){

    std::vector<glm::vec3> skVertices;

    skVertices.push_back(vec3(-size, size, size));
    skVertices.push_back(vec3(size, size, size));
    skVertices.push_back(vec3(-size, -size, size));
    skVertices.push_back(vec3(size, -size, size));
    skVertices.push_back(vec3(size, size, -size));
    skVertices.push_back(vec3(size, -size, -size));
    skVertices.push_back(vec3(-size, size, -size));
    skVertices.push_back(vec3(-size, -size, -size));

    return skVertices;

}

vector<unsigned short> skybox::Indices(){

    vector<unsigned short> indices;

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(2);

    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(3);

    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(5);

    indices.push_back(6);
    indices.push_back(0);
    indices.push_back(7);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(7);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(3);
    indices.push_back(5);
    indices.push_back(7);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(6);

    return indices;

}