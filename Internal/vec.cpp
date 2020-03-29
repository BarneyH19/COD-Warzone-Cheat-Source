#include "vec.h"

Vector3 Vector3::operator-(const Vector3& rhs) const
{
	Vector3 ret;
	ret.x = this->x - rhs.x;
	ret.y = this->y - rhs.y;
	ret.z = this->z - rhs.z;
	return ret;
}

float Vector3::Dot(const Vector3& rhs) const
{
	return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
}