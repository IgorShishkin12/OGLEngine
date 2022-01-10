#pragma once
#include "stb_image.h"
#include<string>
#include<iostream>
#include<array>
class Polygon
{
	struct Vert
	{
		float x, y, z;
	};
	Vert v1, v2, v3;
	bool hasTex;
	int id;
	struct Color
	{
		float r=0, g=0, b=0, a=0;
	};
	Color c;
	class Tex
	{
		int height;
		int width;
		int nrChannels;
		unsigned char* data;
		std::string path="";
		
	public:

		Tex()
		{

		}
		void clearData()
		{
			stbi_image_free(data);
		}
		void reTex(std::string Path="")
		{
			if (Path == "")
			{
				if (path == "")
					std::cerr << "enter tex path";
				Path = path;
			}
			else
			path = Path;
			clearData();
			data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
			if (!data)
			{
				std::cerr << "Failed to load texture";
			}
		}
	};
	Tex t;
public:
	Polygon(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z, float v3x, float v3y, float v3z, bool hastex = 0) :
		v1{ v1x,v1y,v1z }, v2{ v2x,v2y,v2z }, v3{ v3x,v3y,v3z }, hasTex{ hastex }
	{

	}
	void setColor(float r, float g, float b, float a)
	{
		if (!hasTex)
			c = Color{ r,g,b,a };
		else
			std::cerr << "trying to set color, but using texture";
	}

	void setTex(std::string path)
	{
		if (hasTex)
			t.reTex(path);
		else
			std::cerr << "trying to set texture, but using color";
	}
	std::array<float, 12> get()
	{
		return(std::array<float, 12>{ v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, (hasTex) ? 1.0f : 0.0f, float(id), 0.0f });
	}

};