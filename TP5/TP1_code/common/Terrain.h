#ifndef TERRAIN_H
#define TERRAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "Transform.h"
#include "EntityRoot.h"

class Terrain:public EntityRoot
{
public:
	
	Terrain();

	Terrain(int resolution, int taille, float offset, const string path, const string heightmap_path, int mode, uint programID);

	Terrain(vector<vec3> sommets, vector<unsigned short> indices, float offset, const string path, const string heightmap_path, int mode, uint programID);
	
	float FindMinZ();
	float FindMaxZ();
	float FindMaxX();
	float FindMinX();
	
	std::vector<glm::vec3> Vertices(int resolution, int taille, float offset);

	std::vector<unsigned short> Indices(int resolution, int taille, float offset);

	std::vector<std::vector<unsigned short>> Triangles(std::vector<unsigned short> indices);

	std::vector<vec2> compute_uv(vector<vec3> vertices);

	void InfinitePlane(float offset);

};

#endif