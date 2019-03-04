#pragma once
#include"glm\glm.hpp"
struct Ray
{
	glm::vec3 origin, direction;
	Ray(glm::vec3 a, glm::vec3 b) :origin(a), direction(b) {};
};