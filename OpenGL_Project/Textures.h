#pragma once

#include "Box.h"
#include "Sphere.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Textures
{
	unsigned int ID_texDataSph;
public:
	std::vector<Box> boxes;
	std::vector<Sphere> spheres;
	std::vector<float> out;
	long spheresSize = 0;
	long boxesSize = 0;
	const int sizey = 1;
	Textures(std::vector<Box> bs,std::vector<Sphere> ss):
		boxes{bs},spheres{ss}
	{
	glGenTextures(1, &ID_texDataSph);
	glBindTexture(GL_TEXTURE_2D, ID_texDataSph);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//см ниже
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//пиксель на всей области одного цвета
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
	void send()
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, out.size(), sizey, 0, GL_RGBA, GL_FLOAT, &out[0]);                  //gl_rgba32f ненормализует

	}

};