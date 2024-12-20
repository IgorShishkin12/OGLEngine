#pragma once
#include <array>
#include"Matrix.h"
class Box
{
	typedef float angle;

	angle a, b, c;
	std::array<float, 6> sicoss{ 0,1,0,1,0,1 };
	std::array<float, 3> sizes;//x,y,z ������,������,�������
	std::array<float, 3> position;//x,y,z
	bool isDefPosInTex;

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
	Basis base;

	std::pair<unsigned long, unsigned long> posInTex;
public:
	static const int size = 12;
	long materialID;
	int materialClass;
	Box(float sizex=0, float sizey=0, float sizez=0, float x=0, float y=0, float z=0, float a1=0, float b1=0, float c1=0) :
		a{ a1 }, b{ b1 }, c{ c1 }, sizes{ sizex,sizey,sizez }, position{ x,y,z }
	{
		if (a != 0 || b != 0 || c != 0)
		{
			sicoss = { sin(a),cos(a),sin(b),cos(b),sin(c),cos(c) };
		}
		else
		{
			sicoss={ 0,1,0,1,0,1 };
		}
	}
	std::array<float, 9> getRmxForBox()
	{
		MatrixF mx1(3), mx2(3), mx3(3), mx4(3);
		mx1 = changeRMx3('x', (float)a);
		mx2 = changeRMx3('y', (float)b);
		mx3 = changeRMx3('z', (float)c);
		mx4 = mx1 * mx2;
		mx4 = mx4 * mx3;
		return std::array<float, 9>{mx4.get(1, 1), mx4.get(1, 2), mx4.get(1, 3), mx4.get(2, 1), mx4.get(2, 2), mx4.get(2, 3), mx4.get(3, 1), mx4.get(3, 2), mx4.get(3, 3) };
	}
	void setMat(long long ID, int cl)
	{
		materialID = ID;
		materialClass = cl;
	}
	std::array<long, 2> getMat()
	{
		return std::array<long, 2>{materialClass, (long)materialID};
	}

	std::array<float, size> get()
	{
		return{ sicoss[0], sicoss[1], sicoss[2], sicoss[3], sicoss[4], sicoss[5],sizes[0],sizes[1],sizes[2],position[0],position[1],position[2] };
	}
	void operator=(Box in)
	{
		this->a = in.a;
		this->b = in.b;
		this->c = in.c;
		this->sicoss = in.sicoss;
		this->sizes = in.sizes;
		this->position = in.position;
		this-> isDefPosInTex= in.isDefPosInTex;
		this->posInTex = in.posInTex;
	}
};
