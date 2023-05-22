#include <vector>
#include <string>
#include <iostream>

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

#include "Transform.h"

Transform::Transform()
{
}

void Transform::updateRotationX(float speed) { this->LocaleMatrix = rotate(this->LocaleMatrix, speed, vec3(1., 0., 0.)); }

void Transform::updateRotationY(float speed) { this->LocaleMatrix = rotate(this->LocaleMatrix, speed, vec3(0., 1., 0.)); }

void Transform::updateRotationZ(float speed) { this->LocaleMatrix = rotate(this->LocaleMatrix, speed, vec3(0., 0., 1.)); }

void Transform::updateScaling(vec3 rescale) { this->LocaleMatrix = scale(this->LocaleMatrix, rescale); }

void Transform::updateTranslateX(vec3 trans) { this->LocaleMatrix = translate(this->LocaleMatrix, trans); }

void Transform::updateTranslateY(vec3 trans) { this->LocaleMatrix = translate(this->LocaleMatrix, trans); }

void Transform::updateTranslateZ(vec3 trans) { this->LocaleMatrix = translate(this->LocaleMatrix, trans); }

void Transform::updateTranslate(vec3 trans)
{
    position += trans;
    this->LocaleMatrix = translate(this->LocaleMatrix, trans);
}

void Transform::identity()
{
    this->LocaleMatrix = mat4(1.f);
}

mat4 Transform::getLocalModelMatrix()
{
    return this->LocaleMatrix;
}
