#pragma once

#include "Box.h"
#include "Sphere.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"Shader.h"

#include <cstdarg>

//все о текстурах: обычно цвета нормализуются, в gl_rgba32f не нормализуются
// параметры:https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
//  texelFetch позволяет передавать в аргументах значения как для массива
// 

class Textures
{
	template<class t1>
	class clr
	{
	public:
		t1 r;
		t1 g;
		t1 b;
		t1 a;
	};
	ShProg &ourShader;
	Sphere sph0;
	Box box0;
	unsigned int id = 0;

	unsigned int idArr[32]{ 0 };
	unsigned int locArr[32]{ 0 };
	GLenum targetArr[32];

	long spheresSize = 0;
	long boxesSize = 0;
	const int sizey = 1;
	vector<float> out;
	vector<float> colorOut;
	int ID_sizeSph = glGetUniformLocation(ourShader.getID(), "sizeSph");
	int ID_sizeSphes = glGetUniformLocation(ourShader.getID(), "SphBeg_to_End");
	int ID_sizeBoxes = glGetUniformLocation(ourShader.getID(), "BoxBeg_to_End");


public:
	vector<clr<float>> colorBox, colorSph;
	std::vector<Box> boxes;
	std::vector<Sphere> spheres;
	Textures(ShProg sdr,std::vector<Box> bs,std::vector<Sphere> ss):
		boxes{bs},spheres{ss},ourShader{sdr}
	{
	}
	void setColors()
	{
		for (auto i : colorSph)
		{
			colorOut.push_back(i.r);
			colorOut.push_back(i.g);
			colorOut.push_back(i.b);
			colorOut.push_back(i.a);
		}
		for (auto i : colorBox)
		{
			colorOut.push_back(i.r);
			colorOut.push_back(i.g);
			colorOut.push_back(i.b);
			colorOut.push_back(i.a);
		}
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
	void createTex(GLenum t1, unsigned int type, int width, int height=0, int depth=0)
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glGenTextures(1, &(idArr[id]));
		glBindTexture(t1, idArr[id]);
		glTexParameteri(t1, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//см ниже
		glTexParameteri(t1, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//пиксель на всей области одного цвета
		if (t1 == GL_TEXTURE_1D || t1 == GL_TEXTURE_BUFFER)
			glTexImage1D(t1, 0, type, width, 0, GL_RGBA, GL_FLOAT, idArr);//заполняется мусором
		else if (t1 == GL_TEXTURE_2D || t1 == GL_TEXTURE_1D_ARRAY || t1 == GL_TEXTURE_RECTANGLE)
		{
			glTexImage2D(t1, 0, type, width, height, 0, GL_RGBA, GL_FLOAT, 0);//заполняется мусором

		}
		else if (t1 == GL_TEXTURE_3D || t1 == GL_TEXTURE_2D_ARRAY)
			glTexImage3D(t1, 0, type, width,height,depth, 0, GL_RGBA, GL_FLOAT, idArr);//заполняется мусором
		glUniform1i(locArr[id], id);
		targetArr[id] = t1;
		id++;
	}
	void setloc(string str)
	{
		locArr[id] = glGetUniformLocation(ourShader.getID(), str.c_str());
	}
	void setloc(int id2)
	{
		locArr[id] = id2;
	}
	//условие ввода: сначала размеры затем отступ
	//в зависимости от размерности выбирается какие переменные куда поставить
	//пример:
	//insDataTex<2, float>(targetid, data, sizex, sizey, offsetx, offsety);
	// можно было бы сделать элипсис, но там не понятно кончились ли числа или мы их не ввели так как они 0
	// 
	template<class cl1,int dim = 1,GLenum type = GL_FLOAT>
	void insDataTex(int id2, cl1* dataIn, int in1 = 0, int in2 = 0, int in3 = 0, int in4 = 0, int in5 = 0, int in6 = 0 )
	{
		glActiveTexture(GL_TEXTURE0 + id2);
		glBindTexture(targetArr[id2], idArr[id2]);
		if (dim == 1)
			glTexSubImage1D(targetArr[id2], 0, in2,in1, GL_RGBA, type, dataIn);
		else if (dim == 2)
			glTexSubImage2D(targetArr[id2], 0, in3, in4, in1, in2, GL_RGBA, type, dataIn);
		else if (dim == 3)
			glTexSubImage3D(targetArr[id2], 0, in4,in5,in6,in1,in2,in3, GL_RGBA, type, dataIn);
	}
	void sendTex(int id)
	{

		glActiveTexture(GL_TEXTURE0+id);
		glBindTexture(targetArr[id], idArr[id]);
	}
	void start()
	{	
		//временно:
		colorOut.resize(1);

		setloc("Content");
		createTex(GL_TEXTURE_2D, GL_RGBA32F, out.size(), sizey);
		insDataTex<float, 2>(id-1, &out[0], out.size(), sizey);

		setloc("ColorsTex");
		createTex(GL_TEXTURE_2D, GL_RGBA32F, 1,2);
		insDataTex<float, 2>(id-1, &colorOut[0], 1,2);



	}
		GLuint texture;
	void texarr()
	{
		float texels[32] =
		{

0.9879,	0.2128, 0.5376,	0.0735,
0.0478,	0.9165,0.8748,	0.0358,
0.4566,	0.3258,0.3934,	0.7363,
0.0639,	0.6707,0.8550,	0.8131,
0.0259,	0.8566,0.2673,	0.6458,
0.3680,	0.8245,0.2727,	0.0121,
0.3627,	0.1939,0.1525,	0.1523,
0.8642,	0.9577,0.2093,	0.4590
		};

		setloc("texture_array");
		createTex(GL_TEXTURE_2D_ARRAY, GL_RGBA8, 2,2,2);
		insDataTex<float, 3>(id-1, texels, 2,2,2);

	}
	template<class  cl1,int len>
	void testData(GLenum target)
	{

		cl1 texelso[len]{ 0 };
		glGetTexImage(target, 0, GL_RGBA, GL_FLOAT, texelso);

		for (int i = 0; i < len; ++i)
		{
			cout << texelso[i] << "\t";
		}
	}
	//bool addTex(int layer, float* data)
	//{
	//	//reference
	//	//https://habr.com/ru/post/457380/
	//	// https://www.khronos.org/opengl/wiki/Array_Texture
	//	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage3D.xhtml
	//	glTextureSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, /*width*/512, /*height*/512, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	return true;
	//}
	void send()
	{



		//testData<float, 32>(GL_TEXTURE_2D_ARRAY);

		sendTex(1);
		sendTex(0);
		sendTex(2);

		glUniform2i(ID_sizeSph, sph0.size, sizey);
		glUniform2i(ID_sizeSphes, 0, spheresSize);
		glUniform2i(ID_sizeBoxes, spheresSize+1, boxesSize + spheresSize+1);
		//for (auto q : out)
		//{
		//	cout << q << "\n";
		//}
	}
	void sendData()
	{

		glUniform2i(ID_sizeSph,sph0.size, sizey);
		glUniform2i(ID_sizeSphes, 0, spheresSize/4);
		glUniform2i(ID_sizeBoxes, spheresSize/4, boxesSize/4);
		//cout << spheresSize<<"\n";
	}

};