#include "Enemy.h"

Enemy::Enemy()
{
	
}

Enemy::~Enemy()
{
	
}

Enemy::Enemy(std::vector<Vertex> vbd, Transform *t,
	char * n, Rigidbody b, GLuint va, glm::vec3 fi, collType coll, glm::vec3 dim) :GameObject(vbd, t, n, b, va, fi, coll, dim)
{
	moveable = true;
	maxSpeed = 0.002;
	direction = 1;
	type = "Enemy";
}
void Enemy::calcVelocity(float dt)
{
	body.velocity.y += (body.force.y * dt) / (body.mass);
	clampVelocity({ 1,2,1 });
}

void Enemy::move(float dt)
{
	transform.location += body.velocity * dt;
}

void Enemy::applyPhysics(float dt, GameObject * floor)
{
	body.force.y -= 1;
	calcVelocity(dt);
	//std::cout << "Enemy: " << body.velocity.x << std::endl;
}
