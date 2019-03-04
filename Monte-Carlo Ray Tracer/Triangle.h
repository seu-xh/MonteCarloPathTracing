#pragma once
#include"glm\glm.hpp"
#include"Model.h"
#include"Ray.h"
#include"BoundingBox.h"

struct Triangle
{
	Mesh *mesh;
	std::vector<Vertex>vertices;
	BBOX bbox;
	void GetBbox();
	glm::vec3 GetMid();
	bool IsIntersectWithRay(Ray ray, float* t, float* u, float* v);
	Triangle(Mesh* mesh, Vertex a, Vertex b, Vertex c);
};