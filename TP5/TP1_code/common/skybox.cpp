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

skybox::skybox(){}

skybox::skybox(vector<string> faces, vector<vec3> skyboxVertices, vector<unsigned short> indices, int mode){
    this->sk = loadSkybox(faces);
    this->sommets = skyboxVertices;
    this->indices = indices;
}