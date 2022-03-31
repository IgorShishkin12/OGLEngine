#pragma once
#include <array>

class Sphere
{
public:
	static const int size = 12;
	float radius;
	float x, y, z;
	struct Basis
	{
		struct Vec
		{
			float x, y, z;
			void operator=(Vec in)
			{
				x = in.x;
				y = in.y;
				z = in.z;
			}
		};
		Vec v1, v2, v3;
		void operator=(Basis in)
		{
			v1 = in.v1;
			v2 = in.v2;
			v3 = in.v3;
		}
	};
	Basis b;
	long materialID;
	int materialClass;
	Sphere(float radius1 = 1, float x1 = 0, float y1 = 0, float z1 = 0) :
		radius{ radius1 }, x{ x1 }, y{ y1 }, z{ z1 }
	{
	}
	Sphere* setBasis(float v1x=1, float v1y=0, float v1z=0, float v2x=0, float v2y=1, float v2z=0)
	{
		b.v1 = Basis::Vec{ v1x,v1y,v1z };
		b.v2 = Basis::Vec{ v2x,v2y,v2z };
		return this;
	}
	std::array<float, size> getArr()
	{
		return std::array<float, size>{radius, x, y, z, b.v1.x,b.v1.y,b.v1.z,b.v2.x,b.v2.y,b.v2.z};
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
		this->b = in.b;
	}
};
