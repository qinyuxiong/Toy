#ifndef _TGEOMETRYSHADER_H_
#define _TGEOMETRYSHADER_H_

class TGeometryShader
{
public:
	TGeometryShader();

	~TGeometryShader();

	TGeometryShader(const TGeometryShader &GeometryShader);

	void operator = (const TGeometryShader &GeometryShader);
};

#endif