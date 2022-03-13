#pragma once
#include <array>

class Sphere
{
public:
	float radius;
	float x, y, z;
	float xrot, yrot, zrot;
	long materialID;
	int materialClass;
	const int size = 8;
	Sphere(float radius1 = 1, float x1 = 0, float y1 = 0, float z1 = 0, float xrot1 = 0, float yrot1 = 0, float zrot1 = 0) :radius{ radius1 }, x{ x1 }, y{ y1 }, z{ z1 }, xrot{ xrot1 }, yrot{ yrot1 }, zrot{ zrot1 }
	{
	}
	std::array<float, 8> getArr()
	{
		return std::array<float, 8>{radius, x, y, z, xrot, yrot, zrot, 0};
	}
	void setMat(long long ID, int cl)
	{
		materialID = ID;
		materialClass = cl;
	}
	std::array<long,2> getMat()
	{
		return std::array<long, 2>{materialClass, (long)materialID};
	}
	void operator=(Sphere in)
	{
		this->radius = in.radius;
		this->x = in.x;
		this->y = in.y;
		this->z = in.z;
		this->xrot = in.xrot;
		this->yrot = in.yrot;
		this->zrot = in.zrot;
	}
};
