#ifndef _TVERTEX_H_
#define _TVERTEX_H_

#include "utility.h"

class TVertex
{
public:
	TVertex();

	TVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float r, float g, float b, float alpha);

	TVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz);

	TVertex(const TVector &vector, const TMapAddress &map, const TVector &normal, const Color &color);

	TVertex(const TVector &vector, const TMapAddress &map, const TVector &normal);

	~TVertex();

	TVertex(const TVertex &vertex);

	TVertex& operator = (const TVertex &vertex);

	TVector m_vecPoint;
	TMapAddress m_nAddress;
	TVector m_vecN;
	Color m_Color;
};

#endif