#pragma once
#include<vector>
#include"glm\glm.hpp"
#include"glm\gtc\matrix_transform.hpp"
#include"glm\gtc\type_ptr.hpp"
#include"assimp\Importer.hpp"
#include"assimp\scene.h"
#include"assimp\postprocess.h"
#include"BoundingBox.h"
//#include"Vertex.h"

enum  Material
{
	DIFF,SPEC,REFR,PLASTIC
};
class Mesh
{
public:
	//Mesh();
	//~Mesh();
	std::vector<Vertex>vertices;
	std::vector<int>indices;
	Material material;
	glm::vec3 ka, kd, ks;
	double shin, refra,opaque;
	Mesh(std::vector<Vertex>&v, std::vector<int>&i, Material m, glm::vec3 a, glm::vec3 d, glm::vec3 s) :vertices(v), indices(i), material(m), ka(a), kd(d), ks(s) {};
	Mesh(std::vector<Vertex>&v, std::vector<int>&i, Material m) :vertices(v), indices(i), material(m), ka(glm::vec3(0)), kd(glm::vec3(0)), ks(glm::vec3(0)) {};
private:
};

class Triangle
{
public:
	Mesh *mesh;
	std::vector<Vertex>vertices;
	BBOX bbox;
	void GetBbox();
	glm::vec3 GetMid();
	bool IsIntersectWithRay(Ray ray, float* t, float* u, float* v);
	Triangle(Mesh* mesh, Vertex a, Vertex b, Vertex c);
};

/*class Triangle
{
public:
	Mesh * mesh;
	std::vector<Vertex>vertices;
	BBOX bbox;
	void GetBbox();
	glm::vec3 GetMid();
private:
};*/

class  Model {
public:
	Model(std::string path) { this->loadModel(path); };
	~Model() {};
	std::vector<Mesh> meshes;
private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::string directory;
};


