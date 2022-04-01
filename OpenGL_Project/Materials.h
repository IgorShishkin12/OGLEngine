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
	auto getData()
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

	auto getData()
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

	auto getData()
	{
		return std::array<float, 4> {float(texid),0,0,0};
	}
};
class Portal
{
	long targetObj;//aka functionNumber
	long id;//aka number in list of ...
public:
	Portal(long target,long id1) :
		targetObj{ target },id{id1}
	{

	}
	auto getData()
	{
		return std::array<float, 4> {float(targetObj), float(id) , 0, 0};
	}
};