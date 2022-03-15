#pragma once
#include "stb_image.h"
#include<string>
#include<iostream>
#include<array>
class Triangle
{
	struct Vert
	{
		float x, y, z;
	};
	Vert v1, v2, v3;

	//class Tex
	//{
	//	int height;
	//	int width;
	//	int nrChannels;
	//	unsigned char* data;
	//	std::string path="";
	//	
	//public:
	//	Tex()
	//	{
	//	}
	//	void clearData()
	//	{
	//		stbi_image_free(data);
	//	}
	//	void reTex(std::string Path="")
	//	{
	//		if (Path == "")
	//		{
	//			if (path == "")
	//				std::cerr << "enter tex path";
	//			Path = path;
	//		}
	//		else
	//		path = Path;
	//		clearData();
	//		data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	//		if (!data)
	//		{
	//			std::cerr << "Failed to load texture";
	//		}
	//	}
	//};
	//Tex t;
public:
	long materialID;
	int materialClass;
	Triangle(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z, float v3x, float v3y, float v3z) :
		v1{ v1x,v1y,v1z }, v2{ v2x,v2y,v2z }, v3{ v3x,v3y,v3z }
	{

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
	std::array<float, 12> get()
	{
		return(std::array<float, 12>{ v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, 0, 0, 0 });
	}

};