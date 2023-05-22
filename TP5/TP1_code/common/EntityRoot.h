#ifndef ENTITYROOT_H
#define ENTITYROOT_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#include "Transform.h"

class EntityRoot
{
public :
	std::vector<glm::vec3> sommets;
	std::vector<glm::vec3> normales;
	std::vector<unsigned short> indices;
	std::vector<glm::vec2> uvs;
	vector<vec2> heightmap_uvs;
	std::vector<std::vector<unsigned short>> triangles;
	Transform transform;
	uint texture, heightmap, sk;
	uint vertexbuffer, indexbuffer, uvbuffer, normalbuffer;
	EntityRoot* parent = nullptr;
	vector<EntityRoot*> children;
	int mode = 0;
	int texturemode = 0;

	EntityRoot();

	void addChildren(EntityRoot &child);

	void removeChildren(EntityRoot &child);

	void updateSelfAndChild();

	void setUpTexture();

	void InitBuffers();

	void bindBuffers();

	void drawEntity(uint programID);

	void clearBuffers();

};

#endif