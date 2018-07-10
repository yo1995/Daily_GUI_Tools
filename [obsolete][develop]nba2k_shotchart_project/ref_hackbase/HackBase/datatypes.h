#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#include "hackbase.h"
#include "string.h"

enum D3DVersion {
	D3DVersion_None,
	D3DVersion_D3D9,
	D3DVersion_D3D10,
	D3DVersion_D3D11
};

enum WindowsVersion {
	WindowsVersion_Unknown,
	WindowsVersion_XP,
	WindowsVersion_Vista,
	WindowsVersion_Win7,
	WindowsVersion_Win8,
	WindowsVersion_Win10
};

enum MenuCursor {
	MenuCursor_Arrow
};

struct ShaderConstants {
	float TransformMatrix[16];
	float Color[4];
};

struct COLOR_VERTEX {
	D3DXVECTOR3 Position;
	D3DXCOLOR Color;
};

enum text_alignment { 
	lefted, 
	centered, 
	righted 
};

class Rect {
public:
	int x1, y1, x2, y2;

	Rect(int x1, int y1, int x2, int y2) {
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
};

class Size {
public:
	int x, y;

	Size() {
		this->x = 0;
		this->y = 0;
	}

	Size(int x, int y) {
		this->x = x;
		this->y = y;
	}

	Size operator+(const Size& p) const {
		return Size(x + p.x, y + p.y);
	}

	Size& operator+=(const Size& p) {
		this->x += p.x;
		this->y += p.y;
		return *this;
	}

	Size operator-(const Size& p) const {
		return Size(x - p.x, y - p.y);
	}

	Size& operator-=(const Size& p) {
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}
};

class Point {
public:
	int x, y;

	Point() {
		this->x = 0;
		this->y = 0;
	}

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	Point operator+(const Point& p) const {
		return Point(x + p.x, y + p.y);
	}

	Point& operator+=(const Point& p) {
		this->x += p.x;
		this->y += p.y;
		return *this;
	}

	Point operator-(const Point& p) const {
		return Point(x - p.x, y - p.y);
	}

	Point& operator-=(const Point& p) {
		this->x -= p.x;
		this->y -= p.y;
		return *this;
	}
};

class Vector3 {
public:
	float x, y, z;

	Vector3(void) { set(0.f, 0.f, 0.f); }
	Vector3(float x, float y, float z) { set(x, y, z); }
	Vector3& operator=(float s) {
		x = s;
		y = s;
		z = s;
		return *this;
	}
	Vector3 operator-(void) const {
		return Vector3(-x, -y, -z);
	}
	Vector3& operator-=(const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	Vector3& operator+=(const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	Vector3& operator*=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	Vector3& operator*=(Vector3 v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector3 operator*(float s) const {
		return Vector3(x * s, y * s, z * s);
	}
	Vector3 operator*(Vector3 v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	Vector3& operator/=(float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
	Vector3& operator/=(Vector3 v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	Vector3 operator/(float s) const {
		return Vector3(x / s, y / s, z / s);
	}
	Vector3 operator/(Vector3 v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}
	float dot(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	Vector3 cross(const Vector3& v) const {
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	float length(void) const {
		return sqrt(x * x + y * y + z * z);
	}
	float angle(const Vector3& v) const {
		return acos(this->dot(v) / (length() * v.length()));
	}
	void normalize(void) {
		float len = length();
		x /= len;
		y /= len;
		z /= len;
	}
	void scale(const Vector3& v, float magnitude) {
		x = v.x * magnitude;
		y = v.y * magnitude;
		z = v.z * magnitude;
	}

	operator float *() {
		return &x;
	}

	operator const float *() const {
		return &x;
	}


private:
	void set(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	};
};

#endif