#include"Triangle.h"


Triangle::Triangle(Mesh* mesh, Vertex a, Vertex b, Vertex c) {
	this->mesh = mesh;
	this->vertices.push_back(a);
	this->vertices.push_back(b);
	this->vertices.push_back(c);
}

glm::vec3 Triangle::GetMid() {
	return (this->bbox.min + this->bbox.max)*glm::mat3(0.5);
}

void Triangle::GetBbox() {
	this->bbox.ComputeBBOX(this->vertices);
}

bool Triangle::IsIntersectWithRay(Ray ray, float* t, float* u, float* v) {

	glm::vec3 v0, v1, v2;
	glm::vec3 orig, dir;
	v0 = this->vertices[0].position;
	v1 = this->vertices[1].position;
	v2 = this->vertices[2].position;
	orig = ray.origin;
	dir = ray.direction;
	// E1
	glm::vec3 E1 = v1 - v0;

	// E2
	glm::vec3 E2 = v2 - v0;

	// P
	glm::vec3 P = glm::cross(dir, E2);


	// determinant
	double det = glm::dot(E1, P);


	// keep det > 0, modify T accordingly
	glm::vec3 T;
	if (det >0)
	{
		T = orig - v0;
	}
	else
	{
		T = v0 - orig;
		det = -det;
	}

	// If determinant is near zero, ray lies in plane of triangle
	if (det < 0.0000001)
		return false;

	// Calculate u and make sure u <= 1
	*u = glm::dot(T, P);

	if (*u < 0.0f || *u > det)
		return false;

	// Q
	glm::vec3 Q = glm::cross(T, E1);
		

	// Calculate v and make sure u + v <= 1
	*v = glm::dot(dir, Q);

	if (*v < 0.0f || *u + *v > det)
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = glm::dot(E2, Q);


	if (*t<0.0000001) {
		return false;
	}
	double fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;
	if ((*t)<0.0000001) {
		return false;
	}

	return true;


}