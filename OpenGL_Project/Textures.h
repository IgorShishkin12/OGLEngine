#pragma once
#include "Box.h"
#include "Sphere.h"
#include <vector>
class Textures
{
public:
	std::vector<Box> boxes;
	std::vector<Sphere> spheres;
	std::vector<float> out;
	long spheresSize = 0;
	long boxesSize = 0;
	Textures(std::vector<Box> bs,std::vector<Sphere> ss):
		boxes{bs},spheres{ss}
	{
	}
	void makeOut()
	{
		out.clear();
		for (auto i : spheres)
		{
			auto qq = i.getArr();
			for (auto j : qq)
			{
				out.push_back(j);
			}
		}
		spheresSize = out.size();
		for (auto i : boxes)
		{
			auto qq = i.get();
			for (auto j : qq)
			{
				out.push_back(j);
			}
		}
		boxesSize = out.size() - spheresSize;
	}

};