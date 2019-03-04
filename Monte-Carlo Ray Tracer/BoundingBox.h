#pragma
#include<vector>
#include"glm\glm.hpp"
#include"Vertex.h"
#include"Ray.h"

struct BBOX
{
	glm::vec3 min, max;
	void ComputeBBOX(const std::vector<Vertex>&v);
	//void ComputeBBOX(const std::vector<Triangle*>&tri);
	void IncludeVertex(Vertex v);
	int GetLongestAxis();
	//void ComputeSpanAlongDir(glm::vec3 Dir, const std::vector<Vertex>&vertices, double& min, double& max);
	BBOX(glm::vec3 a, glm::vec3 b) :min(a), max(b) {};
	BBOX() {};
	bool IsIntersect(Ray ray);
};



