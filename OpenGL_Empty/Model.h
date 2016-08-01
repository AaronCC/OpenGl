#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "GameObject.h"
class Model
{
public:
	Model();
	std::pair<std::vector<GameObject::Vertex>, GLuint> Model::genVertBufData(std::string objFileName);
	~Model();

	bool buffer(std::vector<GameObject*> objects, std::string objFile);
	void render(GameObject * obj);

private:
	struct VertInd {
		GLuint locInd;
		GLuint uvInd;
		GLuint normInd;
	};
	GLuint vertBuf;
	GLuint vertArr;
	unsigned int vertCount;
	std::vector <unsigned int> locInds;
	std::vector<GameObject::Vertex> locs;
};

