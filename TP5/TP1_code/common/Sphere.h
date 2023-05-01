#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#include "EntityRoot.h"

class Sphere:public EntityRoot{

public:

	vector<vector<vec3>> resolutions_vertices;
	vector<vector<unsigned short>> resolutions_indices;
	vector<vector<vec2>> resolutions_uvs;

	Sphere();

	Sphere(const string textpath, int slices, int stacks, float radius);

	void sphere(float radius, int nb_slices, int nb_stacks, vector<vec3> &vertices, vector<unsigned short> &indices);

	vector<vec2> compute_uv(vector<vec3> vertices);

	void DifferentModel();

	void updateBuffers(vector<vec3> vertices, vector<unsigned short> indices, vector<vec2> uv);

};






#endif