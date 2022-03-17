#pragma once
#include <array>
class Material1
{
	float r, g, b;
	float reflectability;
	float maxTanDiff;
public:
	Material1(float r1, float g1, float b1, float reflectability1 = 99999, float MTD = 0.0) :
		r{ r1 }, g{ g1 }, b{ b1 },  reflectability{ reflectability1 },maxTanDiff{MTD}
	{

	}
	std::array<float, 8> getData()
	{
		return std::array<float, 8> {r, g, b, 0, reflectability, maxTanDiff, 0, 0};
	}
};
//brc for bright parameter
class LightSource
{

	float r, g, b;
	float brc1, brc2, brc3;
public:
	LightSource(float r1, float g1, float b1, float brc11=1, float brc21=1, float brc31=1) :
		r{ r1 }, g{ g1 }, b{ b1 }, brc1{ brc11 }, brc2{ brc21 }, brc3{ brc31 }
	{

	}

	std::array<float, 8> getData()
	{
		return std::array<float, 8> {r, g, b, 0, brc1, brc2, brc3, 0};
	}
};
class hasTex
{
	long texid;
public:
	hasTex(long texid1) :
		texid{ texid1 }
	{

	}

	std::array<float, 4> getData()
	{
		return std::array<float, 4> {0,0,0,0};
	}
	std::array<long, 2> getDataAb()
	{
		return std::array<long, 2> {texid,0};
	}
};