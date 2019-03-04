#pragma once
//#include"Model.h"
#include"Intersection.h"
struct BVH {
	BBOX bbox;
	BVH *left, *right;
	std::vector<Triangle*>triangles;
	BVH* build(std::vector<Triangle*>&triangles);
	bool IsIntersectWithRay(const Ray& ray, Intersection &inter, double &minDis);
	BBOX GetBbox();
};