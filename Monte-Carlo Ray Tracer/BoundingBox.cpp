#include"BoundingBox.h"
#include<algorithm>
#define INF 1000000000;


void BBOX::IncludeVertex(Vertex v) {//现有的包围盒拓展后包含另一个点
	this->min.x = std::min(v.position.x, this->min.x);
	this->min.y = std::min(v.position.y, this->min.y);
	this->min.z = std::min(v.position.z, this->min.z);
	this->max.x = std::max(v.position.x, this->max.x);
	this->max.y = std::max(v.position.y, this->max.y);
	this->max.z = std::max(v.position.z, this->max.z);
}


/*void BBOX::ComputeBBOX(const std::vector<Vertex>&v) {
	double minX = INF;
	double maxX = -INF;
	ComputeSpanAlongDir(glm::vec3(1, 0, 0), v, minX, maxX);

	double minY = INF;
	double maxY = -INF;
	ComputeSpanAlongDir(glm::vec3(0, 1, 0), v, minY, maxY);

	double minZ = INF;
	double maxZ = -INF;
	ComputeSpanAlongDir(glm::vec3(0, 0, 1), v, minZ, maxZ);

	this->min = glm::vec3(minX, minY, minZ);
	this->max = glm::vec3(maxX,maxY,maxZ);

}*/

void BBOX::ComputeBBOX(const std::vector<Vertex>&v) {
	int len = v.size();
	if (len == 0)
		return;
	this->max = this->min = v[0].position;
	for (int i = 1; i < len; i++)
	{
		IncludeVertex(v[i]);
	}
}

/*void BBOX::ComputeBBOX(const std::vector<Triangle*>&tri) {
	if (tri.size() == 0)
		return;
	for (int i = 0; i < tri.size(); i++)
	{
		for (int j = 0; j < tri[i]->vertices.size(); j++)
		{
			if (i == 0 && j == 0)
			{
				this->max = this->min = tri[i]->vertices[j].position;
			}
			else
				IncludeVertex(tri[i]->vertices[j]);
		}
	}
}*/

bool BBOX::IsIntersect(Ray ray) {

	double tmin = 0.0f;
	double tmax = INF;
	if (abs(ray.direction.x) < 0.0000001) //If the ray parallel to the YZ plane  
	{
		//If the ray is not within AABB box, then not intersect
		if (ray.origin.x < min.x || ray.origin.x > max.x)
			return false;
	}
	else
	{
		double ood = 1.0 / ray.direction.x;
		double t1 = (min.x - ray.origin.x) * ood;
		double t2 = (max.x - ray.origin.x) * ood;

		if (t1 > t2)
			std::swap(t1, t2);

		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;

		if (tmin > tmax) return false;
	}


	if (abs(ray.direction.y) < 0.0000001)  
	{
		if (ray.origin.y < min.y || ray.origin.y > max.y)
			return false;
	}
	else
	{
		double ood = 1.0f / ray.direction.y;
		double t1 = (min.y - ray.origin.y) * ood;
		double t2 = (max.y - ray.origin.y) * ood;

		if (t1 > t2)
			std::swap(t1, t2);

		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;

		if (tmin > tmax) return false;
	}

 
	if (abs(ray.direction.z) < 0.0000001) //If the ray parallel to the plane  
	{
		if (ray.origin.z < min.z || ray.origin.z > max.z)
			return false;
	}
	else
	{
		double ood = 1.0f / ray.direction.z;
		double t1 = (min.z - ray.origin.z) * ood;
		double t2 = (max.z - ray.origin.z) * ood;

		if (t1 > t2)
			std::swap(t1, t2);

		if (t1 > tmin) tmin = t1;
		if (t2 < tmax) tmax = t2;

		if (tmin > tmax) return false;
	}

	return true;
}

int BBOX::GetLongestAxis() {//得到该包围盒最长的轴，建立bvh时可根据此进一步划分
	glm::vec3 Dis = this->max - this->min;
	if (Dis.x >= Dis.y&&Dis.x >= Dis.z)
		return 0;//x-axis
	if (Dis.y >= Dis.x&&Dis.y >= Dis.z)
		return 1;//y-axis
	if (Dis.z >= Dis.x&&Dis.z >= Dis.y)
		return 2;//z-axis
}