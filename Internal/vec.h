#pragma once
struct Vector2
{
	float x;
	float y;
};

struct Vector3
{
public:
	Vector3 operator-(const Vector3& rhs) const;
	float Dot(const Vector3& rhs) const;
public:
	float x;
	float y;
	float z;
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};

struct Matrix3x4
{
	float m[3][4];
};

struct Matrix4x3
{
	float m[4][3];
};


struct Matrix4x4
{
	union { Vector4 v[4]; float m[4][4]; struct { Vector4 right; Vector4 up; Vector4 forward; Vector4 trans; }; };
};