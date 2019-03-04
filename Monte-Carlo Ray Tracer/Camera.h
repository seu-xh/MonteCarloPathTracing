#pragma once
#include"glm\glm.hpp"
#include"Ray.h"
struct Camera
{
	glm::vec3 position;
	glm::vec3 front,up,right;

	float fov,fovscale;
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float fov) :position(position), front(front), up(up), fov(fov) {
		this->right = glm::cross(front, up);
		this->fovscale = tan(fov*0.5*3.1415926 / 180)*2.0;
	}
	Ray GenerateRay(float x, float y);
};

Ray Camera::GenerateRay(float x, float y) {
	glm::vec3 right = this->right*glm::mat3((x - 0.5)*this->fovscale);
	glm::vec3 up = this->up*glm::mat3((y - 0.5)*this->fovscale);
	Ray ray(this->position, glm::normalize(this->front + right + up));
	return ray;
}