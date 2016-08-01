#pragma once
#include "GameObject.h"
class Player : public GameObject
{
public:
	Player();
	Player(std::vector<Vertex> vbd, Transform * t, char * n, Rigidbody b, GLuint va, glm::vec3 fi, collType coll, glm::vec3 dim);
	~Player();
	void calcVelocity(float dt) override;
	void move(float dt) override;
	void applyPhysics(float dt, GameObject * floor) override;
	void checkColls(const GameObject * floor);
	void jump();
	bool jumping;
	void moveIn(int direction);
};

