#include"Acceleration_bvh.h"
#include<iostream>
#define error 0.00001

BVH* BVH::build(std::vector<Triangle*>&triangles) {
	if (triangles.size() == 0)
		return NULL;
	BVH* root = new BVH();
	root->triangles = triangles;

	root->bbox=root->GetBbox();


	root->left = root->right = NULL;
	if (triangles.size() <= 4)//if triangle.size()<=4 no need to build recursively
		return root;

	//寻找划分kdtree的依据
	glm::vec3 mid(0);
	for (int i = 0; i < triangles.size(); i++)
		mid += triangles[i]->GetMid()*glm::mat3(1.0 / triangles.size());
	glm::vec3 t(1.0);
	t = t * glm::mat3(0.5);
	//std::cout << t.x << " " << t.y << " " << t.z << std::endl;

	std::vector<Triangle*> left;
	std::vector<Triangle*> right;

	int LongestAxis = root->bbox.GetLongestAxis();
	//std::cout << LongestAxis << std::endl;
	for (int i = 0; i < triangles.size(); i++)
	{
		if (LongestAxis == 0)
			triangles[i]->GetMid().x <= mid.x ? right.push_back(triangles[i]) : left.push_back(triangles[i]);
		if (LongestAxis == 1)
			triangles[i]->GetMid().y <= mid.y ? right.push_back(triangles[i]) : left.push_back(triangles[i]);
		if (LongestAxis == 2)
			triangles[i]->GetMid().z <= mid.z ? right.push_back(triangles[i]) : left.push_back(triangles[i]);
	}


	if (left.size() == 0 || right.size() == 0)
		return root;

	root->right = build(right);
	root->left = build(left);
	//root->right =build(right);


	return root;




}

bool BVH::IsIntersectWithRay(const Ray& ray, Intersection &inter, double &minDis) {//相当于后续遍历BVH树
	if (this->bbox.IsIntersect(ray))
	{
		bool IsHit = false;
	

		if (this->left || this->right)
		{
			bool HitLeft = this->left->IsIntersectWithRay(ray, inter, minDis);
			bool HitRight = this->right->IsIntersectWithRay(ray, inter, minDis);
			return HitLeft || HitRight;
		}


		else//叶子节点
		{
			float t=0, u=0, v=0;
			for (int i = 0; i < this->triangles.size(); i++)
			{
				if (this->triangles[i]->IsIntersectWithRay(ray, &t, &u, &v))
				{
					if (t < minDis&&t>error)
					{
						IsHit = true;
						minDis = t;
						inter.isNull = false;
						inter.position = ray.origin + ray.direction*glm::mat3(minDis);
						inter.normal = this->triangles[i]->vertices[0].normal*glm::mat3(1 - u - v) + this->triangles[i]->vertices[1].normal*glm::mat3(u) + this->triangles[i]->vertices[2].normal*glm::mat3(v);
						inter.normal = glm::normalize(inter.normal);
						inter.mesh = this->triangles[i]->mesh;
					}
				}
			}
			if (IsHit)
				return true;
		}

			

	}

	return false;

}

BBOX BVH::GetBbox() {
	BBOX bbox;
		if (this->triangles.size() == 0)
			return bbox;
		for (int i = 0; i < this->triangles.size(); i++)
		{
			for (int j = 0; j < this->triangles[i]->vertices.size(); j++)
			{
				if (i == 0 && j == 0)
				{
					bbox.max = bbox.min = this->triangles[i]->vertices[j].position;
				}
				else
					bbox.IncludeVertex(this->triangles[i]->vertices[j]);
			}
		}
		return bbox;
}

