#ifndef _FRAGMENTSHADER_H_
#define _FRAGMENTSHADER_H_

#include "TPipeLine.h"

class Device;
class TVertex;

class TFragmentShader
{
public:
	TFragmentShader();

	~TFragmentShader();

	TFragmentShader(const TFragmentShader &FragmentShader);

	TFragmentShader& operator = (const TFragmentShader &FragmentShader);

	void SortVertex(TVertex &a, TVertex &b, TVertex &c);

	void TraverseTriangle(const TVertex &a, const TVertex &b, const TVertex &c, const vector<TLightSource *> &lightSources, const TModel &model, RENDER_TYPE nRenderType, LIGHT_MODEL nLightModel);

	void DrawTopFlatTriangle(const TVertex &a, const TVertex &b, const TVertex &c, const vector<TLightSource *> &lightSources, const TModel &model);

	void DrawDownFlatTriangle(const TVertex &a, const TVertex &b, const TVertex &c, const vector<TLightSource *> &lightSources, const TModel &model);

	void DrawScanLine(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model);

	void DrawScanLineT(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model);

	void DrawScanLinePB(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model);

	void DrawScanLinePBT(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources,  const TModel &model);

	void SetDevice(Device *pDevice);

	// calc fragment color by light(Blin-Phong Light Model)
	void CalcFragmentColorWithLight(TVector &point, TVector &normal, Color &result, const TLightSource &LightSource, const TVector &Eye, const Materail &materail);

	void ReverseScreenMapping(TVector &Result);

	void ReversePerspectiveDivision(TVector &Result);

	Device *m_pDevice;

	void (TFragmentShader::*m_fpDrawLineFunc)(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model);
};

#endif