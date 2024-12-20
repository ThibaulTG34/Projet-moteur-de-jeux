// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <typeinfo>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "EntityRoot.h"
#include "BBOX.h"

EntityRoot::EntityRoot()
{
}

// Met a jour les matrices locales et globales
void EntityRoot::updateSelfAndChild()
{
	if (this->parent)
	{
		this->transform.ModelMatrix = this->parent->transform.ModelMatrix * this->transform.getLocalModelMatrix();
	}
	else
	{
		this->transform.ModelMatrix = this->transform.getLocalModelMatrix();
	}

	for (auto &&child : this->children)
	{
		child->updateSelfAndChild();
	}
}

// Ajout enfant
void EntityRoot::addChildren(EntityRoot &child)
{
	this->children.push_back(&child);
	child.parent = this;
}

void EntityRoot::removeChildren(EntityRoot &child)
{
	// for(auto c : this->children)
	// {
	// 	if (c->parent == child.parent)
	// 	{
	// 		c
	// 	}
	// }
	// this->children.push_back(&child);
	// child.parent = this;
}

/*RENDERING*/
void EntityRoot::InitBuffers()
{
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, this->sommets.size() * sizeof(vec3), &sommets[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, this->normales.size() * sizeof(vec3), &normales[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void EntityRoot::bindBuffers()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexbuffer);
}

// Dessine les objets
void EntityRoot::drawEntity(uint programID)
{
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUniform1i(glGetUniformLocation(programID, "mode"), this->mode);

	this->InitBuffers();
	if (this->texture != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		if (this->mode == 1)
		{
			glUniform1i(glGetUniformLocation(programID, "texture_terrain"), 0);
		}
		else if (this->mode == 3)
		{
			glUniform1i(glGetUniformLocation(programID, "texture_roche"), 0);
		}
		else if (this->mode == 4)
		{
			glUniform1i(glGetUniformLocation(programID, "texture_peau"), 0);
		}
		else if (this->mode == 5)
		{
			glUniform1i(glGetUniformLocation(programID, "texture_bois"), 0);
		}
		else if (this->mode == 6)
		{
			glUniform1i(glGetUniformLocation(programID, "texture_arbre"), 0);
		}
	}

	if (this->heightmap != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, heightmap);
		glUniform1i(glGetUniformLocation(programID, "heightmap"), 1);
	}

	if (this->sk != -1)
	{
		glActiveTexture(GL_TEXTURE0 + 1);

		glUniform1i(glGetUniformLocation(programID, "sktext"), 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->sk);
	}

	this->bindBuffers();

	glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, &this->transform.ModelMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_SHORT, (void *)0);
	glDisableVertexAttribArray(0);
	for (unsigned int i = 0; i < this->children.size(); i++)
	{
		children[i]->drawEntity(programID);
	}
}

void EntityRoot::clearBuffers()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &indexbuffer);
	glDeleteBuffers(1, &normalbuffer);
}

void EntityRoot::removeChildren(EntityRoot *child)
{
	for (unsigned int i = 0; i < this->children.size(); i++)
	{
		if (this->children[i] == child)
		{
			this->children[i]->parent == nullptr;
			this->children.erase(this->children.begin() + i);
		}
	}
}
