#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

struct Transform
{
    glm::mat4 LocaleMatrix = glm::mat4(1.0f);
    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    glm::vec3 position;

	Transform();

    void updateRotationX(float speed);

    void updateRotationY(float speed);

    void updateRotationZ(float speed);

    void updateScaling(vec3 rescale);

    void updateTranslateX(vec3 trans);

    void updateTranslateY(vec3 trans);

    void updateTranslateZ(vec3 trans);

    void updateTranslate(vec3 trans);

    mat4 getLocalModelMatrix();

};

#endif