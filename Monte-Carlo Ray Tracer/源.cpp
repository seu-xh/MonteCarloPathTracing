#include<algorithm>
#include<iostream>
#include"Acceleration_bvh.h"
#include <stdint.h>
#include <stdio.h>
#include <tchar.h>
#include"Camera.h"
#include<time.h>
#include"vector3.hpp"
#include"erand.hpp"
#include<omp.h>//cpu并行
Model model("scene01.obj");
const int Width = 400;
const int Height = 400;
const int samples = 10000;
Camera camera(glm::vec3(0, 4.5, 14.5), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 50);
//Camera camera(glm::vec3(1, 4, 20), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 50);
#define INF 1000000000;
#define PI 3.14159265358979323846
BVH *root;

std::vector<Vector3>ResultPixels;
void CreateBVH() {
	std::vector<Triangle*> Scene;
	Mesh* _mesh;
	for (int i = 0; i < model.meshes.size(); i++)
	{
		_mesh = &model.meshes[i];
		for (int j = 0; j < _mesh->indices.size() - 2; j += 3)
		{
			Triangle *tri=new Triangle(_mesh, _mesh->vertices[_mesh->indices[j]], _mesh->vertices[_mesh->indices[j + 1]], _mesh->vertices[_mesh->indices[j + 2]]);
			Scene.push_back(tri);	
		}
	}
	BVH tmp;
	root =tmp.build(Scene);
}


Vector3 MonteCarloPathTracing(Ray ray, int depth, unsigned short *Xi) {
	depth++;
	Intersection result;
	double inf = INF;
	if (!root->IsIntersectWithRay(ray, result, inf)) return Vector3(0.0);
	//cout << result.distance << endl;
	Vector3 ambient(result.mesh->ka);
	if (ambient.Max()>0.5) {//光源
		return ambient;
	}

	Vector3 f = result.mesh->kd;//diffuse
	double p = f.Max();

	Vector3 normal_real = Vector3(result.normal).Dot(ray.direction) < 0 ? result.normal : Vector3(result.normal)*-1.0;
	if (depth > 5) {
		if (erand48(Xi) < p)	f = f * (1 / p);
		else return ambient;
	}

	if (depth > 20) {//深度限制
		return ambient;
	}
	switch (result.mesh->material) {//根据交点材质决定递归path tracing
	case DIFF: {//diffuse材质
		double r1 = 2.0 * PI*erand48(Xi);
		double r2 = erand48(Xi);
		double r2_sqrt = sqrt(r2);
		Vector3 w = normal_real;
		Vector3 u = (fabs(w.x) > 0.1 ? Vector3(0.0, 1.0, 0.0) : Vector3(1.0, 0.0, 0.0)).Cross(w).Normalize();
		Vector3 v = w.Cross(u);
		Vector3 dir = ((u*cos(r1)*r2_sqrt + v * sin(r1)*r2_sqrt + w * sqrt(1 - r2))).Normalize();
		return ambient + f * (MonteCarloPathTracing(Ray(result.position, glm::vec3(dir.x, dir.y, dir.z)), depth, Xi));//xiugai
		break;
	}
	case SPEC: {//反射材质

		Vector3 dir_ref = Vector3(ray.direction) - Vector3(result.normal) * 2 * Vector3(result.normal).Dot(ray.direction);
		return ambient + MonteCarloPathTracing(Ray(result.position, glm::vec3(dir_ref.x, dir_ref.y, dir_ref.z)), depth, Xi);//

		break;
	}
	case REFR: {//透射材质
		Vector3 dir_ref = Vector3(ray.direction) - Vector3(result.normal) * 2 * Vector3(result.normal).Dot(ray.direction);
		Ray ray_refl(result.position, glm::vec3(dir_ref.x, dir_ref.y, dir_ref.z));//
		bool into = Vector3(result.normal).Dot(normal_real)>0;
		double nc = 1.0;//真空
		double nt = result.mesh->refra;
		double nnt = into ? nc / nt : nt / nc;
		double ddn = Vector3(ray.direction).Dot(normal_real)*-1;//入射角余弦
		double sin_2_t = (1 - ddn * ddn)*nnt*nnt;
		if (sin_2_t > 1.0)//全内反射
			return ambient + f * MonteCarloPathTracing(ray_refl, depth, Xi);
		double sin_t = sqrt(sin_2_t);
		double cos_t = sqrt(1 - sin_2_t);
		Vector3 refr_dir = Vector3(normal_real) * (-1 * cos_t) + (Vector3(ray.direction) + normal_real * ddn).Normalize()*sin_t;
		Ray ray_refr(result.position, glm::vec3(refr_dir.x, refr_dir.y, refr_dir.z));//
		double F0 = (nc - nt)*(nc - nt) / ((nc + nt)*(nc + nt));
		double c = 1 - (into ? ddn : refr_dir.Dot(normal_real) * -1);
		double Fe = F0 + (1 - F0) * c * c * c * c * c;//菲涅尔反射
		double Fr = 1 - Fe;//菲涅尔折射

		if (depth > 2) {
			double P = 0.25 + 0.5 * Fe;
			if ((erand48(Xi)) < P)
				return MonteCarloPathTracing(ray_refl, depth, Xi) * (Fe / P);
			else
				return MonteCarloPathTracing(ray_refr, depth, Xi) * (Fr / (1 - P));
		}
		else {
			return MonteCarloPathTracing(ray_refl, depth, Xi) * Fe
				+ MonteCarloPathTracing(ray_refr, depth, Xi) * Fr;
		}
		break;
	}
	case PLASTIC: {//塑料材质
		double r1 = 2.0 * PI*erand48(Xi);
		double r2 = erand48(Xi);
		double r2_sqrt = sqrt(r2);
		Vector3 w = normal_real;
		Vector3 u = (fabs(w.x) > 0.1 ? Vector3(0.0, 1.0, 0.0) : Vector3(1.0, 0.0, 0.0)).Cross(w).Normalize();
		Vector3 v = w.Cross(u);
		Vector3 dir = ((u*cos(r1)*r2_sqrt + v * sin(r1)*r2_sqrt + w * sqrt(1 - r2))).Normalize();
		Vector3 ndir = Vector3(ray.direction) - Vector3(result.normal) * 2 * (Vector3(result.normal).Dot(ray.direction));
		return ambient + result.mesh->opaque*(f*MonteCarloPathTracing(Ray(result.position, glm::vec3(dir.x, dir.y, dir.z)), depth, Xi))
			+ (1 - result.mesh->opaque)*(f*MonteCarloPathTracing(Ray(result.position, glm::vec3(ndir.x, ndir.y, ndir.z)), depth, Xi));
	}
	default:
		break;
	}
	return Vector3(0.0);
}


void Render() {
	ResultPixels.resize(Height*Width);

    #pragma omp parallel for schedule(dynamic, 1) //cpu并行
	for (int y = 0; y < Height; ++y)
	{
		fprintf(stderr, "\rRendering  %5.2f%%", 100.0 * y / (Height - 1));
		double newy = 1 - y / (double)Height;
		unsigned short Xi[3] = { 0, 0, y*y*y };//erand48（）的随机数
       // #pragma omp parallel for
		for (int x = 0; x < Width; x++)
		{
			double newx = x / (double)Width;
			Ray ray = camera.GenerateRay(newx, newy);
			Vector3 color(0, 0, 0);
          //  #pragma omp parallel for
			for (int n = 0; n < samples; n++)
			{
				color += MonteCarloPathTracing(ray, 0, Xi)*(1.0/(double)samples);
			}
			ResultPixels[y*Width+x]=color;
		}
	}
 }

uint8_t ToByte(double a) {//将（0-1）颜色值转成255表示的rgb
	a = a > 1 ? 1 : (a < 0 ? 0 : a);
	return (uint8_t)(pow(a, 1.0 / 2.2) * 255);//补偿pow（a,1/2.2）
}


void ImageOut(const std::string &fname="default.ppm") {//将渲染后的结果写入文件
	FILE *fp;
	fopen_s(&fp, fname.c_str(), "w");
	fprintf(fp, "P3\n%d %d\n%d\n", Width, Height, 255);
	for (int i = 0; i < Width*Height; ++i)
		fprintf(fp, "%d %d %d ", ToByte(ResultPixels[i].x), ToByte(ResultPixels[i].y), ToByte(ResultPixels[i].z));
	fclose(fp);
}

int main() {
	CreateBVH();//建立BVH
	clock_t start = clock();
	Render();
	clock_t end = clock();
	std::cout << std::endl;
	std::cout << "piture:" << Width << "*" << Height << " samples:" << samples << std::endl;
	std::cout << "It costs " << (end - start) / (double)CLOCKS_PER_SEC <<"s"<<std::endl;
	ImageOut("10000SamplesScene01.ppm");
	system("pause");
	return 0;
}