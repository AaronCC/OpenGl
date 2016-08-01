#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(std::vector<Vertex> vbd, Transform *t,
	char *n, Rigidbody b, GLuint va, glm::vec3 fi, collType coll, glm::vec3 dim)
{
	verBufData = vbd;
	transform = *t;
	name = n;
	body = b;
	vertArr = va;
	forceIn = fi;
	collider = coll;
	dimensions = dim;
	onGround = false;
	if (name == "images/coin.png")
		type = "Coin";
}

GameObject::~GameObject()
{
}
float pythag(glm::vec3 pos1, glm::vec3 pos2)
{
	return std::sqrt(std::pow((pos1.x - pos2.x), 2) + std::pow((pos1.y - pos2.y), 2));
}
void GameObject::calcVelocity(float dt)
{

}
void GameObject::move(float dt)
{
}
void GameObject::applyPhysics(float dt, GameObject*floor)
{
}
void GameObject::applyGravity(float dt, GameObject *floor)
{
	if (!(transform.location.y - dimensions.y == floor->transform.location.y))
		body.force.y = body.force.y - 1;
}
void GameObject::applyFriction(float dt)
{
	float sign = (std::abs(body.velocity.x) / body.velocity.x);
	if (body.velocity.x != 0)
	{
		float force = sign * 1;
		body.force.x -= force;
	}

}
void GameObject::clampVelocity(glm::vec3 max)
{
	body.velocity.x = body.velocity.x > max.x ? max.x : body.velocity.x;
	body.velocity.y = body.velocity.y > max.y ? max.y : body.velocity.y;
	body.velocity.z = body.velocity.z > max.z ? max.z : body.velocity.z;
	body.velocity.x = body.velocity.x < -1 * max.x ? -1 * max.x : body.velocity.x;
	body.velocity.y = body.velocity.y < -1 * max.y ? -1 * max.y : body.velocity.y;
	body.velocity.z = body.velocity.z < -1 * max.z ? -1 * max.z : body.velocity.z;
}
void GameObject::clampLocation()
{
	transform.location.x = transform.location.x < -1 ? -1 : transform.location.x;
	transform.location.y = transform.location.y < -1 ? -1 + dimensions.y : transform.location.y;
	transform.location.x = transform.location.x + dimensions.x > 1 ?
		1 - dimensions.x : transform.location.x;
	transform.location.y = transform.location.y > 1 ?
		1 : transform.location.y;
}
bool GameObject::collidesWith(const GameObject * obj)
{
	glm::vec3 center1 = { transform.location.x + (dimensions.x / 2),
						transform.location.y - (dimensions.y / 2),
						transform.location.z + (dimensions.z / 2) };

	glm::vec3 center2 = { obj->transform.location.x + (obj->dimensions.x / 2),
						obj->transform.location.y - (obj->dimensions.y / 2),
						obj->transform.location.z + (obj->dimensions.z / 2) };

	if (collider == Colliderless || obj->collider == Colliderless)
		return false;
	else if (collider == Sphere && obj->collider == Sphere)
	{
		if (pythag({ center1.x, center1.y, center1.z }, { center2.x, center2.y, center2.z })
	> (dimensions.x + obj->dimensions.x) / 2)
			return false;
	}
	else if (collider == AxisAlignedBB && obj->collider == AxisAlignedBB)
	{
		if (std::abs(center1.x - center2.x) >= (dimensions.x + obj->dimensions.x) / 2)
			return false;
		if (std::abs(center1.y - center2.y) >= (dimensions.y + obj->dimensions.y) / 2)
			return false;
		//if (std::abs(center1.z - center2.z) >= (dimensions.z + obj->dimensions.z) / 2)
			//return false;
	}
	else if ((collider == AxisAlignedBB && obj->collider == Sphere) || (collider == Sphere && obj->collider == AxisAlignedBB))
	{
		float dist = 0;
		if (collider == AxisAlignedBB)
		{
			// x
			if (center2.x < transform.location.x)
			{
				dist += std::pow((transform.location.x - center2.x), 2);
			}
			else if (center2.x > transform.location.x)
			{
				dist += std::pow((center2.x - (transform.location.x + dimensions.x)), 2);
			}
			// y
			if (center2.y < transform.location.y)
			{
				dist += std::pow((transform.location.y - center2.y), 2);
			}
			else if (center2.y > transform.location.y)
			{
				dist += std::pow((center2.y - (transform.location.y + dimensions.y)), 2);
			}
			// z
			if (center2.z < transform.location.z)
			{
				dist += std::pow((transform.location.z - center2.z), 2);
			}
			else if (center2.z > transform.location.z)
			{
				dist += std::pow((center2.z - (transform.location.z + dimensions.z)), 2);
			}
		}
		else
		{
			// x
			if (center1.x < obj->transform.location.x)
			{
				dist += std::pow((obj->transform.location.x - center1.x), 2);
			}
			else if (center1.x > obj->transform.location.x)
			{
				dist += std::pow((center1.x - (obj->transform.location.x + dimensions.x)), 2);
			}
			// y
			if (center1.y < obj->transform.location.y)
			{
				dist += std::pow((obj->transform.location.y - center1.y), 2);
			}
			else if (center1.y > obj->transform.location.y)
			{
				dist += std::pow((center1.y - (obj->transform.location.y + dimensions.y)), 2);
			}
			// z
			if (center1.z < obj->transform.location.z)
			{
				dist += std::pow((obj->transform.location.z - center1.z), 2);
			}
			else if (center1.z > obj->transform.location.z)
			{
				dist += std::pow((center1.z - (obj->transform.location.z + dimensions.z)), 2);
			}
		}
	}
	return true;
}
