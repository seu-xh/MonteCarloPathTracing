
#pragma once
#include<math.h>
#include<stdio.h>

struct Vector3 {

	Vector3(double a = 0) : x(a), y(a), z(a) {}
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
	Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}
	Vector3(glm::vec3 v):x(v.x), y(v.y), z(v.z) {}
	

	inline Vector3 &operator=(const Vector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}
	inline Vector3 operator+(const Vector3 &v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	inline Vector3 operator-(const Vector3 &v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	inline Vector3 operator*(const Vector3 &v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	inline Vector3 operator/(const Vector3 &v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}
	inline Vector3 operator+(double a) const {
		return Vector3(x + a, y + a, z + a);
	}
	inline Vector3 operator-(double a) const {
		return Vector3(x - a, y - a, z - a);
	}
	inline Vector3 operator*(double a) const {
		return Vector3(x * a, y * a, z * a);
	}
	inline Vector3 operator/(double a) const {
		const double inv_a = 1.0 / a;
		return Vector3(x * inv_a, y * inv_a, z * inv_a);
	}
	friend inline Vector3 operator+(double a, const Vector3 &v) {
		return Vector3(v.x + a, v.y + a, v.z + a);
	}
	friend inline Vector3 operator-(double a, const Vector3 &v) {
		return Vector3(v.x - a, v.y - a, v.z - a);
	}
	friend inline Vector3 operator*(double a, const Vector3 &v) {
		return Vector3(v.x * a, v.y * a, v.z * a);
	}
	friend inline Vector3 operator/(double a, const Vector3 &v) {
		const double inv_a = 1.0 / a;
		return Vector3(v.x * inv_a, v.y * inv_a, v.z * inv_a);
	}

	inline Vector3 &operator+=(const Vector3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	inline Vector3 &operator-=(const Vector3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	inline Vector3 &operator*=(const Vector3 &v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	inline Vector3 &operator/=(const Vector3 &v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	inline Vector3 &operator+=(double a) {
		x += a;
		y += a;
		z += a;
		return *this;
	}
	inline Vector3 &operator-=(double a) {
		x -= a;
		y -= a;
		z -= a;
		return *this;
	}
	inline Vector3 &operator*=(double a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	inline Vector3 &operator/=(double a) {
		const double inv_a = 1.0 / a;
		x *= inv_a;
		y *= inv_a;
		z *= inv_a;
		return *this;
	}

	inline double Dot(const Vector3 &v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	inline Vector3 Cross(const Vector3 &v) const {
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}


	friend inline Vector3 Min(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}
	friend inline Vector3 Max(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}


	
	inline double operator[](size_t i) const {
		return raw[i];
	}
	inline double &operator[](size_t i) {
		return raw[i];
	}


	inline double Min() const {
		return (x < y && x < z) ? x : ((y < z) ? y : z);
	}
	inline double Max() const {
		return (x > y && x > z) ? x : ((y > z) ? y : z);
	}

	inline double Norm2_squared() const {
		return x * x + y * y + z * z;
	}
	inline double Norm2() const {
		return sqrt(Norm2_squared());
	}
	inline Vector3 &Normalize() {
		const double a = 1 / Norm2();
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	union {
		struct {
			double x, y, z;
		};
		double raw[3];
	};
};