#include "Camera.h"



Camera::Camera()
{
	zoom = 1.f;
	width = 800;
	height = 800;
	fovy = 3.14159f * 0.4f / zoom;
	aspect = (float)width / (float)height;
	zNear = 0.01f;
	zFar = 1000.f;
	transform.location = { 0,0,1.5 };
}


Camera::~Camera()
{
}

void Camera::calcCamMat()
{
	glm::mat3 rotMat = 
		(glm::mat3)glm::yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z);
	glm::vec3 eye = transform.location;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	lookAtMat = glm::lookAt(eye, center, up);

	perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);

	cameraMat = perspectiveMat *lookAtMat;
}

void Camera::update(float dt)
{
	transform.location += body.velocity * dt;
}
