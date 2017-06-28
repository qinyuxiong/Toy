#include "TVertex.h"

TVertex::TVertex()
{

}

TVertex::TVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float r, float g, float b, float alpha) : m_vecPoint(x, y, z, 1), m_nAddress(u, v), m_vecN(nx, ny, nz, 0), m_Color(r, g, b, alpha)
{
}

TVertex::TVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : m_vecPoint(x, y, z, 1), m_nAddress(u, v), m_vecN(nx, ny, nz, 0)
{
}

TVertex::TVertex(const TVector &vector, const TMapAddress &map, const TVector &normal, const Color &color) : m_vecPoint(vector), m_nAddress(map), m_vecN(normal), m_Color(color)
{
}

TVertex::TVertex(const TVector &vector, const TMapAddress &map, const TVector &normal) : m_vecPoint(vector), m_nAddress(map), m_vecN(normal)
{
}

TVertex::~TVertex()
{

}

TVertex::TVertex(const TVertex &vertex)
{
	m_vecPoint = vertex.m_vecPoint;
	m_nAddress = vertex.m_nAddress;
	m_vecN = vertex.m_vecN;
	m_Color = vertex.m_Color;
	//m_AmbientColor = vertex.m_AmbientColor;
	//m_SpecularColor = vertex.m_SpecularColor;
}

TVertex& TVertex::operator = (const TVertex &vertex)
{
	if (this != &vertex)
	{
		m_vecPoint = vertex.m_vecPoint;
		m_nAddress = vertex.m_nAddress;
		m_vecN = vertex.m_vecN;
		m_Color = vertex.m_Color;
		//m_AmbientColor = vertex.m_AmbientColor;
		//m_SpecularColor = vertex.m_SpecularColor;
	}
	return *this;
}
