#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <iostream>
#include "ShaderManager.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Player.h"
#include "Camera.h"
class Engine
{
public:
	Engine();
	~Engine();/*
	struct Vertex
	{
		glm::vec3 location;
		glm::vec2 uv;
	};*/
	GameObject obj;
	bool init();
	bool bindImage(char* name);
	bool bufferModel();
	bool gameLoop();
	void GameOver();
	bool useShaders();
	void spawnEnemy(char * name);
	void spawnCoin();
	void applyFriction(GameObject * obj, float dt);
	void calcVelocity(GameObject * obj, float dt);
	void applyGravity(GameObject * obj);
	
	std::vector<GameObject::Vertex> genVertBufData(std::vector<GameObject::Vertex> locs);
private:
	void calcCamTransform();
	float timeCurrent;
	float timePrevious;
	float timeDelta;
	float spawnTime;
	float coinTime;
	float pos;
	float direction;
	unsigned int imageIndex;
	Camera camera;
	GLuint vertBufs;
	std::vector<GameObject*> objects;
	std::map<char*, GLuint> textures;
	std::vector<char*> imageNames;
	FIBITMAP* image;
	FIBITMAP* image32bit;
	GLFWwindow* GLFWwindowPtr;
	GLuint vertArr;
	unsigned int vertCount;
	std::vector <unsigned int> locInds;
	ShaderManager manager;
};

