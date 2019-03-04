#pragma once
#include"glm\glm.hpp"
#include"Model.h"
struct Intersection
{
	glm::vec3 position;
	glm::vec3 normal;
	Mesh *mesh;
	bool isNull = true;
};