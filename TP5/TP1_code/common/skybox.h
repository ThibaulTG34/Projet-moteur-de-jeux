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

    skybox(vector<string> faces, float size, int mode);

    vector<vec3> Vertices(float size);

    vector<unsigned short> Indices();

};

#endif