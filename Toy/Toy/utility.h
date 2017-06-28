#ifndef _OTHERS_H_
#define _OTHERS_H_

#include <Windows.h>
#include "TVector.h"

const float Pi = 3.14159265f;

class TVertex;

struct TMapAddress
{
	TMapAddress(){}
	TMapAddress(float fu, float fv) : u(fu), v(fv){}
	float u, v;
};

struct Point
{
	float x;
	float y;
};

struct Color
{
	Color(){}
	Color(float a, float b, float c, float d) :r(a), g(b), b(c), alpha(d){}
	float r;
	float g;
	float b;
	float alpha;
};

struct Camera
{
	Camera(){}
	Camera(const TVector &a, const TVector &b, const TVector &c) : eye(a), focus(b), up(c){}
	TVector eye;
	TVector focus;
	TVector up;
};

struct TLightSource
{
	TLightSource(){}
	TLightSource(const TVector &Position, const Color &LightColor, const Color &Ambient) :Pos(Position), Color(LightColor), AmbientColor(Ambient){}
	TVector Pos;
	Color AmbientColor; // 模拟环境光
	Color Color;
};

struct Materail
{
	Materail(){}
	Materail(Color a, Color b, Color c) :m_EmessiveColor(a), m_DiffuseColor(b), m_SpecularColor(c){}

	// 材质的放射光颜色
	Color m_EmessiveColor;
	// 材质的漫反射颜色
	Color m_DiffuseColor;
	// 材质的镜面反射颜色
	Color m_SpecularColor;
};

int MAX(int a, int b);

int MIN(int a, int b);

void BresenhamLine(Point &start, Point &end, unsigned int **Buffer, int nColor);

void SwapVertex(TVertex &a, TVertex &b);

void LerpWithX(const TVertex &start, const TVertex &end, TVertex &result);

void LerpWithY(const TVertex &start, const TVertex &end, TVertex &result);

void Lerp(const TVertex &start, const TVertex &end, TVertex &result);

BOOL FuzzyEqual(float a, float b);

void ModificationsColor(Color &index);

#endif