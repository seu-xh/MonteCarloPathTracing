#include"Model.h"
#include"Ray.h"
#include<iostream>
/*
glm::vec3 Triangle::GetMid() {
	return (this->bbox.min + this->bbox.max)*glm::mat3(0.5);
}

void Triangle::GetBbox() {
	this->bbox.ComputeBBOX(this->vertices);
}
*/

Triangle::Triangle(Mesh* mesh, Vertex a, Vertex b, Vertex c) {
	this->mesh = mesh;
	this->vertices.push_back(a);
	this->vertices.push_back(b);
	this->vertices.push_back(c);
	GetBbox();
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

	glm::vec3 E1 = v1 - v0;

	glm::vec3 E2 = v2 - v0;

	glm::vec3 P = glm::cross(dir, E2);



	double det = glm::dot(E1, P);

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

	if (det < 0.0000001)
		return false;
	*u = glm::dot(T, P);

	if (*u < 0.0f || *u > det)
		return false;

	glm::vec3 Q = glm::cross(T, E1);

	*v = glm::dot(dir, Q);

	if (*v < 0.0f || *u + *v > det)
		return false;

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


void Model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout<< "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));
	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene* scene) {
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	glm::vec3 ka, kd, ks;
	ka = kd = ks = glm::vec3(0);
	float shin,refr,opaue=0;
	Material mtl = DIFF;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = glm::normalize(vector);
		vertices.push_back(vertex);
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor4D diffuse;
		aiColor4D specular;
		aiColor4D ambient;
		aiColor4D emission;
		//diffuse is kd,specular is ks,ambient is ka,shininess is Ns,REFRACTI is Ni
		//	cout << mesh->mMaterialIndex << endl;
		unsigned int  max = 1;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
			kd = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
		}
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
			ks = glm::vec3(specular.r, specular.g, specular.b);
		}
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
			ka = glm::vec3(ambient.r, ambient.g, ambient.b);
			ka *= 100.0;
		}


	    aiGetMaterialFloatArray(material, AI_MATKEY_OPACITY, &opaue, &max);
		aiGetMaterialFloatArray(material, AI_MATKEY_REFRACTI, &refr, &max);
		aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shin, &max);
		if (shin>0) {
			mtl = SPEC;
			kd = glm::vec3(1.0);
		}
		if (refr>1.00001) {
			mtl = REFR;
		}
		if (opaue <1)
			mtl = PLASTIC;

	}
	Mesh _mesh(vertices, indices, mtl, ka,kd, ks);
	_mesh.shin = shin;
	_mesh.refra = refr;
	_mesh.opaque = opaue;
	return _mesh;
}