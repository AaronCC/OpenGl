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
#include "GameObject.h"

class Camera
{
public:
	Camera();
	~Camera();

	void calcCamMat();
	void update(float dt);

	GameObject::Rigidbody body;
	GameObject::Transform transform;
	glm::mat4 lookAtMat;

	glm::mat4 perspectiveMat;

	glm::mat4 cameraMat;

	float zoom;
	int width;
	int height;
	float fovy;
	float aspect;
	float zNear;
	float zFar;
};

