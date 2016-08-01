#pragma once
#include "GameObject.h"
class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();

	Enemy(std::vector<Vertex> vbd, Transform * t, char * n, Rigidbody b, GLuint va, glm::vec3 fi, collType coll, glm::vec3 dim);
	float maxSpeed;
	float direction;
	void calcVelocity(float dt) override;
	void move(float dt) override;
	void applyPhysics(float dt, GameObject* floor) override;
};

