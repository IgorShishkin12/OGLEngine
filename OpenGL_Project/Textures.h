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

class Texture
{
	ShProg &ourShader;

	unsigned int id;

	unsigned int idArr[32]{ 0 };
	unsigned int locArr[32]{ 0 };
	GLenum targetArr[32];
	bool isFree[32];
	void searchFree()
	{
		for (int i = 0; i < 32; ++i)
		{
			if (isFree[i])
			{
				id = i;
				return;
			}
		}
		cerr << "no free textures"<<endl;
	}

public:
	array<string, 32> about;
	Texture(ShProg& sdr):
		ourShader{sdr}
	{
	memset(isFree, 1, 32);
	searchFree();
	}
	template<GLuint type = GL_FLOAT, GLuint format = GL_RGBA>
	unsigned int createTex(GLenum t1, unsigned int internalformat, int width, int height=0, int depth=0)
	{
		searchFree();
		glActiveTexture(GL_TEXTURE0 + id);
		glGenTextures(1, &(idArr[id]));
		glBindTexture(t1, idArr[id]);
	/*	int a, b, c;
		float d;*/
		glTexParameteri(t1, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//см ниже
		glTexParameteri(t1, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//пиксель на всей области одного цвета
		if (t1 == GL_TEXTURE_1D || t1 == GL_TEXTURE_BUFFER)
			glTexImage1D(t1, 0, internalformat, width, 0, format, type, 0);//заполняется мусором
		else if (t1 == GL_TEXTURE_2D || t1 == GL_TEXTURE_1D_ARRAY || t1 == GL_TEXTURE_RECTANGLE)
			glTexImage2D(t1, 0, internalformat, width, height, 0, format, type, 0);//заполняется мусором
		else if (t1 == GL_TEXTURE_3D || t1 == GL_TEXTURE_2D_ARRAY)
			glTexImage3D(t1, 0, internalformat, width,height,depth, 0, format, type, 0);//заполняется мусором
		glUniform1i(locArr[id], id);
		targetArr[id] = t1;
		isFree[id] = 0;
		id++;
		return id-1;
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
	// void glTexSubImage3D(	GLenum target,
							//	GLint level,
							//	GLint xoffset,
							//	GLint yoffset,
							//	GLint zoffset,
							//	GLsizei width,
							//	GLsizei height,
							//	GLsizei depth,
							//	GLenum format,
							//	GLenum type,
							//	const void* pixels);
	// 
	template<class cl1,int dim = 1, GLuint type = GL_FLOAT, GLuint format = GL_RGBA>
	void insDataTex(int id2, cl1* dataIn, int in1 = 0, int in2 = 0, int in3 = 0, int in4 = 0, int in5 = 0, int in6 = 0 )
	{
		glActiveTexture(GL_TEXTURE0 + id2);
		glBindTexture(targetArr[id2], idArr[id2]);
		if (dim == 1)
			glTexSubImage1D(targetArr[id2], 0, in2,in1, format, type, dataIn);
		else if (dim == 2)
			glTexSubImage2D(targetArr[id2], 0, in3, in4, in1, in2, format, type, dataIn);
		else if (dim == 3)
		{
			glTexSubImage3D(targetArr[id2], 0, in4,in5,in6,in1,in2,in3, format, type, dataIn);
			//if(in3==2)
			//testData<float, 40*20>(targetArr[id2]);

		}
	}
	void sendTex(int id)
	{

		glActiveTexture(GL_TEXTURE0+id);
		glBindTexture(targetArr[id], idArr[id]);
	}
	template<class  cl1,int len>
	void testData(GLenum target)
	{

		float *texelso=new float[len];
		glGetTexImage(target, 0, GL_RGBA, GL_FLOAT, texelso);
		string ans;
		for (int i = 0; i < len; ++i)
		{
			if (i % 40 == 0)
				ans+="\n";
			ans+= to_string(texelso[i]) + "\t";
		}
		cout << ans<<"\n\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n\n";
	}
	//	//reference
	//	//https://habr.com/ru/post/457380/
	//	// https://www.khronos.org/opengl/wiki/Array_Texture
	//	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexSubImage3D.xhtml
	//	glTextureSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, /*width*/512, /*height*/512, 1, GL_RGBA, GL_UNSIGNED_BYTE, data)
	void delTex(int id)
	{
		glDeleteTextures(1, &idArr[id]);
		locArr[id] = 0;
		targetArr[id] = 0;
		about[id] = "";
		isFree[id] = 1;
	}
	
};

class Textures
{
	Texture texes;
	vector<float> compressedData;
	vector<long>compressedDataAbout,compressedMaterialAbout;
	long long sizeData=0;
	long maxTexSizeX=0, maxTexSizeY=0;
	struct tex
	{
		int height=0;
		int width=0;
		int channels=0;
		vector<float>data{ 1,0 };
	};
	vector<tex> texData;
	struct DataAbout
	{
		long  dataSize;
		long  cellSize;
		long  cellCount;
		int funcionNumber;
		long long  dataBegin;
		long long  dataEnd;	//  [data)
		long long materialIDBegin;
		vector<float> data;
		vector<long> matData;
		DataAbout( long cellSize1, long cellCount1, int funcionNumber1,long long dataBegin1, float *data1) :
			cellSize{cellSize1},cellCount{cellCount1},funcionNumber{funcionNumber1},dataBegin{dataBegin1}
		{
			dataSize = cellSize*cellCount;
			dataEnd = dataBegin + dataSize-1;
			data.reserve(dataSize);
			for (long long i = 0; i < dataSize; ++i)
			{
				data.push_back(*(data1 + i));
			}
		}
		void setMaterials(long long &matIDBeg, long* matID, int* classID)
		{
			materialIDBegin = matIDBeg;
			matIDBeg += cellCount * 2;
			matData.reserve(cellCount * 2);
			for (int i = 0; i < cellCount; ++i)
			{ 
				matData.push_back(classID[i]);
				matData.push_back(matID[i]);

			}
		}
		auto compress()
		{
			return array<long long, 8>{dataSize, cellSize, cellCount, funcionNumber, dataBegin, dataEnd,materialIDBegin,0};
		}
	};
	vector<DataAbout> vecDataAbout;
	int contentid, aboutid, texesid,aboutMatid;
public:
	Textures(ShProg& sdr) :
		texes{ sdr }
	{

	}
	void createTexes()
	{
		texes.setloc("Content");
		contentid=texes.createTex(GL_TEXTURE_2D, GL_RGBA32F, compressedData.size()/4, 1);
		//texes.insDataTex<float, 2>(0, &compressedData[0], compressedData.size(), 1);

		texes.setloc("ContentAbout");
		aboutid = texes.createTex<GL_INT, GL_RGBA_INTEGER>(GL_TEXTURE_2D, GL_RGBA32I, compressedDataAbout.size() / 4, 1);
		//texes.insDataTex<float, 2>(1, &compressedDataAbout[0], compressedDataAbout.size(), 1);

		texes.setloc("texture_array");
		texesid=texes.createTex(GL_TEXTURE_2D_ARRAY, GL_RGBA8, maxTexSizeX, maxTexSizeY, texData.size());
		//texes.insDataTex<float, 3>(2, &texData[0][0], maxTexSizeX,maxTexSizeY,texData.size());

		texes.setloc("AboutMaterial");
		aboutMatid = texes.createTex<GL_INT, GL_RGBA_INTEGER>(GL_TEXTURE_2D, GL_RGBA32I, ceil(compressedMaterialAbout.size() / 4.0), 1);
		//texes.insDataTex<float, 2>(1, &compressedDataAbout[0], compressedDataAbout.size(), 1);
	}

	long addData( long cellSize1,long cellCount1, long functionNumber1,float* data1)
	{
		DataAbout timeTex{ cellSize1,cellCount1,functionNumber1,sizeData,data1 };
		sizeData += timeTex.dataSize;
		for(auto &i : vecDataAbout)
			if (i.funcionNumber == functionNumber1)
			{
				//i = timeTex;
				std::cerr << "you truying to edit with function for adding\n";
				exit(-1);
			}
		vecDataAbout.push_back(timeTex);
		return vecDataAbout.size()-1;
	}
	void addMaterials(long long& matIDBeg, long* matID, int* classID,long id = -1)
	{
		if (id == -1)
			id = vecDataAbout.size() - 1;
		vecDataAbout[id].setMaterials(matIDBeg, matID, classID);
		
	}
	int addTex(int height1, int width1,int channels, const float* data1)
	{
		texData.push_back(tex{height1,width1,channels});
		//texData.back().data.reserve(height1 * width1*channels);
		for (long long i = 0; i < height1 * width1*channels*1LL; ++i)
		{
			texData.back().data.push_back(data1[i]);
		}
		maxTexSizeX = max(maxTexSizeX, texData.back().width);
		maxTexSizeY = max(maxTexSizeY, texData.back().height);
		return texData.size() - 1;
	}

	void compressTex()
	{
		//vector<float> vec22;
		long j = 0;
		
		for (auto& i : texData)
		{
			j++;
			texes.insDataTex<float, 3>(texesid, &i.data[0], i.width, i.height, 1, 0, 0, j - 1);
				
		}
	}

	void compressData(bool isIns= 1)
	{
		compressedData.clear();
		compressedDataAbout.clear();
		compressedMaterialAbout.clear();
		for (auto& i : vecDataAbout)
		{
			for (auto j : i.data)
				compressedData.push_back(j);
			const array <long long, 8>& time{ i.compress() };
			for (auto j : time)
				compressedDataAbout.push_back(j);
			for (auto j : i.matData)
				compressedMaterialAbout.push_back(j);
		}

		if (isIns)
		{
			texes.insDataTex<float, 2>(contentid, &compressedData[0], compressedData.size() / 4, 1);
			texes.insDataTex<long, 2, GL_INT, GL_RGBA_INTEGER>(aboutid, &compressedDataAbout[0], compressedDataAbout.size() / 4, 1);
			texes.insDataTex<long, 2, GL_INT, GL_RGBA_INTEGER>(aboutMatid, &compressedMaterialAbout[0], ceil(compressedMaterialAbout.size()/ 4.0), 1);
		}
	}

	void sendData();
	void reCompress()
	{
	}

	void send()
	{
		texes.sendTex(contentid);
		texes.sendTex(aboutid);
		texes.sendTex(texesid);
		texes.sendTex(aboutMatid);
	}
};