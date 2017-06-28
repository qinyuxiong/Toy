#include "utility.h"
#include "TVertex.h"

int MAX(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

int MIN(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

void BresenhamLine(Point &start, Point &end, unsigned int **Buffer, int nColor)
{
	int Color = nColor > 0 ? nColor : 0xffffff;
	int x0 = (int)start.x;
	int y0 = (int)start.y;
	int x1 = (int)end.x;
	int y1 = (int)end.y;

	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int x = x0;
	int y = y0;
	int stepX = x0 > x1 ? -1 : 1;
	int stepY = y0 > y1 ? -1 : 1;

	//沿着最长的那个轴前进
	if (dx > dy)
	{
		int e = dy * 2 - dx;
		for (int i = 0; i <= dx; i++)
		{
			Buffer[y][x] = Color;
			x += stepX;
			e += dy;
			if (e >= 0)
			{
				y += stepY;
				e -= dx;
			}
		}
	}
	else
	{
		int e = 2 * dx - dy;
		for (int i = 0; i <= dy; i++)
		{
			Buffer[y][x] = Color;
			y += stepY;
			e += dx;
			if (e >= 0)
			{
				x += stepX;
				e -= dy;
			}
		}
	}
}

void SwapVertex(TVertex &a, TVertex &b)
{
	TVertex index = a;
	a = b;
	b = index;
}

void LerpWithX(const TVertex &start, const TVertex &end, TVertex &result)
{
	if (end.m_vecPoint.m_arrData[0] == start.m_vecPoint.m_arrData[0])
		return;

	float fScale = (result.m_vecPoint.m_arrData[0] - start.m_vecPoint.m_arrData[0]) / (end.m_vecPoint.m_arrData[0] - start.m_vecPoint.m_arrData[0]);
	result.m_vecPoint.m_arrData[1] = (end.m_vecPoint.m_arrData[1] - start.m_vecPoint.m_arrData[1]) * fScale + start.m_vecPoint.m_arrData[1];
	result.m_vecPoint.m_arrData[2] = (end.m_vecPoint.m_arrData[2] - start.m_vecPoint.m_arrData[2]) * fScale + start.m_vecPoint.m_arrData[2];
	result.m_nAddress.u = (end.m_nAddress.u - start.m_nAddress.u) * fScale + start.m_nAddress.u;
	result.m_nAddress.v = (end.m_nAddress.v - start.m_nAddress.v) * fScale + start.m_nAddress.v;
	result.m_vecN.m_arrData[0] = (end.m_vecN.m_arrData[0] - start.m_vecN.m_arrData[0]) * fScale + start.m_vecN.m_arrData[0];
	result.m_vecN.m_arrData[1] = (end.m_vecN.m_arrData[1] - start.m_vecN.m_arrData[1]) * fScale + start.m_vecN.m_arrData[1];
	result.m_vecN.m_arrData[2] = (end.m_vecN.m_arrData[2] - start.m_vecN.m_arrData[2]) * fScale + start.m_vecN.m_arrData[2];
	result.m_Color.r = (end.m_Color.r - start.m_Color.r) * fScale + start.m_Color.r;
	result.m_Color.g = (end.m_Color.g - start.m_Color.g) * fScale + start.m_Color.g;
	result.m_Color.b = (end.m_Color.b - start.m_Color.b) * fScale + start.m_Color.b;
	result.m_Color.alpha = (end.m_Color.alpha - start.m_Color.alpha) * fScale + start.m_Color.alpha;
}

void LerpWithY(const TVertex &start, const TVertex &end, TVertex &result)
{
	if (end.m_vecPoint.m_arrData[1] == start.m_vecPoint.m_arrData[1])
		return;

	float fScale = (result.m_vecPoint.m_arrData[1] - start.m_vecPoint.m_arrData[1]) / (end.m_vecPoint.m_arrData[1] - start.m_vecPoint.m_arrData[1]);
	result.m_vecPoint.m_arrData[0] = (end.m_vecPoint.m_arrData[0] - start.m_vecPoint.m_arrData[0]) * fScale + start.m_vecPoint.m_arrData[0];
	result.m_vecPoint.m_arrData[2] = (end.m_vecPoint.m_arrData[2] - start.m_vecPoint.m_arrData[2]) * fScale + start.m_vecPoint.m_arrData[2];
	result.m_vecPoint.m_arrData[3] = (end.m_vecPoint.m_arrData[3] - start.m_vecPoint.m_arrData[3]) * fScale + start.m_vecPoint.m_arrData[3];
	result.m_nAddress.u = (end.m_nAddress.u - start.m_nAddress.u) * fScale + start.m_nAddress.u;
	result.m_nAddress.v = (end.m_nAddress.v - start.m_nAddress.v) * fScale + start.m_nAddress.v;
	result.m_vecN.m_arrData[0] = (end.m_vecN.m_arrData[0] - start.m_vecN.m_arrData[0]) * fScale + start.m_vecN.m_arrData[0];
	result.m_vecN.m_arrData[1] = (end.m_vecN.m_arrData[1] - start.m_vecN.m_arrData[1]) * fScale + start.m_vecN.m_arrData[1];
	result.m_vecN.m_arrData[2] = (end.m_vecN.m_arrData[2] - start.m_vecN.m_arrData[2]) * fScale + start.m_vecN.m_arrData[2];
	result.m_vecN.m_arrData[3] = (end.m_vecN.m_arrData[2] - start.m_vecN.m_arrData[3]) * fScale + start.m_vecN.m_arrData[3];
	result.m_Color.r = (end.m_Color.r - start.m_Color.r) * fScale + start.m_Color.r;
	result.m_Color.g = (end.m_Color.g - start.m_Color.g) * fScale + start.m_Color.g;
	result.m_Color.b = (end.m_Color.b - start.m_Color.b) * fScale + start.m_Color.b;
	result.m_Color.alpha = (end.m_Color.alpha - start.m_Color.alpha) * fScale + start.m_Color.alpha;
}

//void Lerp(const TVertex &start, const TVertex &end, TVertex &result)
//{
//	if (end.m_vecPoint.m_arrData[1] == start.m_vecPoint.m_arrData[1])
//		return;
//
//	float fScale = (result.m_vecPoint.m_arrData[1] - start.m_vecPoint.m_arrData[1]) / (end.m_vecPoint.m_arrData[1] - start.m_vecPoint.m_arrData[1]);
//	result.m_vecPoint.m_arrData[2] = (end.m_vecPoint.m_arrData[2] - start.m_vecPoint.m_arrData[2]) * fScale + start.m_vecPoint.m_arrData[2];
//	result.m_nAddress.u = (end.m_nAddress.u - start.m_nAddress.u) * fScale + start.m_nAddress.u;
//	result.m_nAddress.v = (end.m_nAddress.v - start.m_nAddress.v) * fScale + start.m_nAddress.v;
//	result.m_vecN.m_arrData[0] = (end.m_vecN.m_arrData[0] - start.m_vecN.m_arrData[0]) * fScale + start.m_vecN.m_arrData[0];
//	result.m_vecN.m_arrData[1] = (end.m_vecN.m_arrData[1] - start.m_vecN.m_arrData[1]) * fScale + start.m_vecN.m_arrData[1];
//	result.m_vecN.m_arrData[2] = (end.m_vecN.m_arrData[2] - start.m_vecN.m_arrData[2]) * fScale + start.m_vecN.m_arrData[2];
//	result.m_Color.r = (end.m_Color.r - start.m_Color.r) * fScale + start.m_Color.r;
//	result.m_Color.g = (end.m_Color.g - start.m_Color.g) * fScale + start.m_Color.g;
//	result.m_Color.b = (end.m_Color.b - start.m_Color.b) * fScale + start.m_Color.b;
//	result.m_Color.alpha = (end.m_Color.alpha - start.m_Color.alpha) * fScale + start.m_Color.alpha;
//}

BOOL FuzzyEqual(float a, float b)
{
	float index = a - b;
	if (index >= -0.0001 && index <= 0.0001)
		return TRUE;
	else
		return FALSE;
}

void ModificationsColor(Color &index)
{
	index.r = index.r > 1.0f ? 1.0f : index.r;
	index.g = index.g > 1.0f ? 1.0f : index.g;
	index.b = index.b > 1.0f ? 1.0f : index.b;
}