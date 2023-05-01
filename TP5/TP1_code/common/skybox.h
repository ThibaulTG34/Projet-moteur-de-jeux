#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "Transform.h"
#include "EntityRoot.h"

using namespace std;
using namespace glm;

class skybox:public EntityRoot
{
public:

    skybox();

    skybox(vector<vector<string>> faces, vector<vec3> skyboxVertices, vector<unsigned short> indices, int mode);

};

#endif