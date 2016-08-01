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
class GameObject
{
public:
	enum collType 
	{
		Colliderless,
		AxisAlignedBB,
		Sphere
	};
	struct Vertex
	{
		glm::vec3 location;
		glm::vec2 uv;
	};
	struct Transform
	{
		glm::vec3 location;
		glm::vec3 rotation;
		glm::vec3 size;
		glm::mat4 objWorldTransform;
	};
	struct Rigidbody {
		glm::vec3 velocity;
		glm::vec3 force;
		float mass;
	};
	GameObject();
	GameObject(std::vector<Vertex> vbd, Transform *t,
		char * n, Rigidbody b, GLuint va, glm::vec3 fi, collType coll, glm::vec3 dim);
	~GameObject();
	std::vector<Vertex> verBufData;
	glm::vec3 dimensions;
	collType collider;
	Transform transform;
	char* name;
	Rigidbody body;
	GLuint vertArr;
	glm::vec3 forceIn;
	std::string type;
	virtual void calcVelocity(float dt);
	virtual void move(float dt);
	virtual void applyPhysics(float dt, GameObject*floor);
	void applyGravity(float dt, GameObject *floor);
	void applyFriction(float dt);
	 void clampLocation();
	void clampVelocity(glm::vec3 max);
	bool collidesWith(const GameObject *obj);
	bool moveable;
	bool onGround;
};

