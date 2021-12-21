#pragma once

#include "Box.h"
#include "Sphere.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"

//все о текстурах: обычно цвета нормализуются, в gl_rgba32f не нормализуются
// параметры:https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
//  texelFetch позволяет передавать в аргументах значения как для массива
// 
class Textures
{
	ShProg &ourShader;
	Sphere sph0;
	Box box0;
	unsigned int ID_texDataSph;
	long spheresSize = 0;
	long boxesSize = 0;
	const int sizey = 1;
	std::vector<float> out;
	int ID_sizeSph = glGetUniformLocation(ourShader.getID(), "sizeSph");
	int ID_sizeSphes = glGetUniformLocation(ourShader.getID(), "SphBeg_to_End");
	int ID_sizeBoxes = glGetUniformLocation(ourShader.getID(), "BoxBeg_to_End");
public:
	std::vector<Box> boxes;
	std::vector<Sphere> spheres;
	Textures(ShProg sdr,std::vector<Box> bs,std::vector<Sphere> ss):
		boxes{bs},spheres{ss},ourShader{sdr}
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
		boxesSize = out.size();
	}
	void send()
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, out.size(), sizey, 0, GL_RGBA, GL_FLOAT, &out[0]);                  //gl_rgba32f ненормализует
		glUniform2i(ID_sizeSph, sph0.size, sizey);
		glUniform2i(ID_sizeSphes, 0, spheresSize);
		glUniform2i(ID_sizeBoxes, spheresSize+1, boxesSize + spheresSize+1);
		for (auto q : out)
		{
			cout << q << "\n";
		}
	}
	void sendData()
	{

		glUniform2i(ID_sizeSph,sph0.size, sizey);
		glUniform2i(ID_sizeSphes, 0, spheresSize/4);
		glUniform2i(ID_sizeBoxes, spheresSize/4, boxesSize/4);
		//cout << spheresSize<<"\n";
	}

};