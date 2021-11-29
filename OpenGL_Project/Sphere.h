#pragma once
#include <array>

class Sphere
{
public:
	float radius;
	float x, y, z;
	float r, g, b,a;
	const int size = 8;
	Sphere(float radius=1, float x=0, float y=0, float z=0, float r=0, float g=0, float b=0, float a=1) :radius{radius},x{x},y{y},z{z},r{r},g{g},b{b},a{}
	{
	}
	std::array<float, 8> getArr()
	{
		return std::array<float, 8>{radius, x, y, z, r, g, b,a};
	}
	void operator=(Sphere in)
	{
		this->radius = in.radius;
		this->x = in.x;
		this->y = in.y;
		this->z = in.z;
		this->r = in.r;
		this->g = in.g;
		this->b = in.b;
		this->a = in.a;
	}
};
