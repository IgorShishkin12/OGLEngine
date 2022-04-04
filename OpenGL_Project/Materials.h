#pragma once
#include <array>
#include <vector>
class Material1
{
	float r, g, b;
	float reflectability;
	float maxTanDiff;
public:
	Material1(float r1, float g1, float b1, float reflectability1 = 99999, float MTD = 0.0) :
		r{ r1 }, g{ g1 }, b{ b1 }, reflectability{ reflectability1 }, maxTanDiff{ MTD }
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
	LightSource(float r1, float g1, float b1, float brc11 = 1, float brc21 = 1, float brc31 = 1) :
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
	std::vector<float>data;
public:
	const long long texid, x, y, ch = 4;
	hasTex(long long texid1, long x1/*aka w*/, long y1/*aka h*/, float* data1) :
		texid{ texid1 }, x{ x1 }, y{ y1 }
	{
		data.resize(x* y* ch);
		long it = -1;
		for (auto& i : data)i = data1[++it];
	}
	hasTex(long texid1, long x1, long y1, long ch1, float* data1) :
		texid{ texid1 }, x{ x1 }, y{ y1 }, ch{ ch1 }
	{
		data.reserve(x* y* ch);
		long it = -1;
		for (auto& i : data)i = data1[++it];
	}
	const auto* d()
	{
		return &data[0];
	}

	auto getData()
	{
		return std::array<float, 4> {float(texid), float(x), float(y), 0};
	}
	
};
class Portal
{
	long targetObj;//aka functionNumber
	long id;//aka number in list of ...
public:
	Portal(long target, long id1) :
		targetObj{ target }, id{ id1 }
	{

	}
	auto getData()
	{
		return std::array<float, 4> {float(targetObj), float(id), 0, 0};
	}
};