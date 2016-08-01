#include "Player.h"

Player::Player()
{
}
Player::Player(std::vector<Vertex> vbd, Transform *t, char * n, Rigidbody b, GLuint va, glm::vec3 fi, collType coll, glm::vec3 dim) :GameObject(vbd, t, n, b, va, fi, coll, dim)
{
	moveable = true;
	type = "Player";
};
Player::~Player()
{
}
 

void Player::calcVelocity(float dt)
{
	if (std::abs((body.force.x  * dt) / (body.mass)) > std::abs(body.velocity.x) && forceIn.x == 0)
		body.velocity.x = 0;
	else
		body.velocity.x += (body.force.x  * dt) / (body.mass);
	body.velocity.y += (body.force.y * dt) / (body.mass);
	body.velocity.z += (body.force.z * dt) / (body.mass);
}

void Player::move(float dt)
{
	transform.location += body.velocity * dt;
	clampLocation();
}

void Player::applyPhysics(float dt, GameObject* floor)
{
	body.force += forceIn;
	applyGravity(dt, floor);
	applyFriction(dt);
	calcVelocity(dt);
	clampVelocity({ 2, 2, 1 });
	//std::cout << "Player: " << body.velocity.x << std::endl;
}
void Player::checkColls(const GameObject* floor)
{
	if (collidesWith(floor))
	{
		transform.location.y += floor->transform.location.y - (transform.location.y - dimensions.y);
		body.velocity.y = 0;
	}
}
void Player::jump()
{
		body.force.y += 500;
		onGround = false;

}

void Player::moveIn(int direction)
{
	forceIn.x =  direction * 5;
}
