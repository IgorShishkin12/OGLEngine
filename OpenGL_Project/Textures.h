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
	unsigned int ID_texDataSph{ 0 };
	unsigned int ID_texColor{ 0 };
	unsigned int texture_array{0};
	long spheresSize = 0;
	long boxesSize = 0;
	const int sizey = 1;
	std::vector<float> out;
	int ID_sizeSph = glGetUniformLocation(ourShader.getID(), "sizeSph");
	int ID_sizeSphes = glGetUniformLocation(ourShader.getID(), "SphBeg_to_End");
	int ID_sizeBoxes = glGetUniformLocation(ourShader.getID(), "BoxBeg_to_End");
	vector<float> colorOut;


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
	void start()
	{	
		//временно:
		colorOut.resize(1);


		glGenTextures(1, &ID_texDataSph);
		glBindTexture(GL_TEXTURE_2D, ID_texDataSph);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//см ниже
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//пиксель на всей области одного цвета

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, out.size(), sizey, 0, GL_RGBA, GL_FLOAT, &out[0]);

		glGenTextures(1, &ID_texColor);
		glBindTexture(GL_TEXTURE_2D, ID_texColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//см ниже
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//пиксель на всей области одного цвета
		if (!colorOut.size()) 
		{
			cout << "colorOut is empty\n";
			colorOut.push_back(0.0);
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1, 1, 0, GL_RGBA, GL_FLOAT, &colorOut[0]);

		glUniform1i(glGetUniformLocation(ourShader.getID(), "Content"), 0);
		glUniform1i(glGetUniformLocation(ourShader.getID(), "ColorsTex"), 1);
	}
	void texarr()
	{
		GLuint texture = 0;

		GLsizei width = 2;
		GLsizei height = 2;
		GLsizei layerCount = 2;
		GLsizei mipLevelCount = 1;

		// Read you texels here. In the current example, we have 2*2*2 = 8 texels, with each texel being 4 GLubytes.
		GLfloat texels[32] =
		{
			// Texels for first image.
			0,   0,   0,   255,
			255, 0,   0,   255,
			0,   255, 0,   255,
			0,   0,   255, 255,
			// Texels for second image.
			255, 255, 255, 255,
			255, 255,   0, 255,
			0,   255, 255, 255,
			255, 0,   255, 255,
		};

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
		// Allocate the storage.
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, layerCount, 0, GL_RGBA, GL_FLOAT, NULL);
		// Upload pixel data.
		// The first 0 refers to the mipmap level (level 0, since there's only 1)
		// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
		// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
		// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_FLOAT, texels);

		// Always set reasonable texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ID_texDataSph);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ID_texColor);

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