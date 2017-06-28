#include "TFragmentShader.h"
#include "TVertex.h"
#include "utility.h"
#include "Device.h"

TFragmentShader::TFragmentShader()
{
	m_pDevice = NULL;
	m_fpDrawLineFunc = NULL;
}

TFragmentShader::~TFragmentShader()
{
	m_pDevice = NULL;
	m_fpDrawLineFunc = NULL;
}

TFragmentShader::TFragmentShader(const TFragmentShader &FragmentShader)
{

}

TFragmentShader& TFragmentShader::operator = (const TFragmentShader &FragmentShader)
{
	return *this;
}

void TFragmentShader::SortVertex(TVertex &v1, TVertex &v2, TVertex &v3)
{
	if (v1.m_vecPoint.m_arrData[1] > v2.m_vecPoint.m_arrData[1] || (v1.m_vecPoint.m_arrData[1] == v2.m_vecPoint.m_arrData[1] && v1.m_vecPoint.m_arrData[0] > v2.m_vecPoint.m_arrData[0]))
		SwapVertex(v1, v2);

	if (v2.m_vecPoint.m_arrData[1] > v3.m_vecPoint.m_arrData[1] || (v2.m_vecPoint.m_arrData[1] == v3.m_vecPoint.m_arrData[1] && v2.m_vecPoint.m_arrData[0] > v3.m_vecPoint.m_arrData[0]))
		SwapVertex(v2, v3);

	if (v1.m_vecPoint.m_arrData[1] > v2.m_vecPoint.m_arrData[1] || (v1.m_vecPoint.m_arrData[1] == v2.m_vecPoint.m_arrData[1] && v1.m_vecPoint.m_arrData[0] > v2.m_vecPoint.m_arrData[0]))
		SwapVertex(v1, v2);
}

void TFragmentShader::TraverseTriangle(const TVertex &a, const TVertex &b, const TVertex &c, const vector<TLightSource *> &lightSources, const TModel &model, RENDER_TYPE nRenderType, LIGHT_MODEL nLightModel)
{
	if (WIRE_FRAME == nRenderType)
	{
		Point IndexA;
		IndexA.x = a.m_vecPoint.m_arrData[0];
		IndexA.y = a.m_vecPoint.m_arrData[1];

		Point IndexB;
		IndexB.x = b.m_vecPoint.m_arrData[0];
		IndexB.y = b.m_vecPoint.m_arrData[1];

		Point IndexC;
		IndexC.x = c.m_vecPoint.m_arrData[0];
		IndexC.y = c.m_vecPoint.m_arrData[1];

		BresenhamLine(IndexA, IndexB, m_pDevice->frameBuffer, 0);
		BresenhamLine(IndexA, IndexC, m_pDevice->frameBuffer, 0);
		BresenhamLine(IndexB, IndexC, m_pDevice->frameBuffer, 0);
	}
	else
	{
		m_fpDrawLineFunc = NULL;
		if (NORMAL_COLOR_FILL == nRenderType)
		{
			if (GROUND_MODEL == nLightModel)
				m_fpDrawLineFunc = &TFragmentShader::DrawScanLine;
			else if (PHONG_BLIN_MODEL == nLightModel)
				m_fpDrawLineFunc = &TFragmentShader::DrawScanLinePB;
		}
		else if (COLOR_FILL_WITH_MAP == nRenderType)
		{
			if (GROUND_MODEL == nLightModel)
				m_fpDrawLineFunc = &TFragmentShader::DrawScanLineT;
			else if (PHONG_BLIN_MODEL == nLightModel)
				m_fpDrawLineFunc = &TFragmentShader::DrawScanLinePBT;
		}

		if (FuzzyEqual(a.m_vecPoint.m_arrData[1], b.m_vecPoint.m_arrData[1]))
		{
			// top_flat_triangle
			DrawTopFlatTriangle(a, b, c, lightSources, model);
		}
		else if (FuzzyEqual(b.m_vecPoint.m_arrData[1], c.m_vecPoint.m_arrData[1]))
		{
			// down_flat_triangle
			DrawDownFlatTriangle(a, b, c, lightSources, model);
		}
		else
		{
			TVertex IndexB = b;
			TVertex vertex;
			vertex.m_vecPoint.m_arrData[1] = IndexB.m_vecPoint.m_arrData[1];
			LerpWithY(a, c, vertex);

			if (vertex.m_vecPoint.m_arrData[0] < IndexB.m_vecPoint.m_arrData[0])
				SwapVertex(vertex, IndexB);

			DrawDownFlatTriangle(a, IndexB, vertex, lightSources, model);

			DrawTopFlatTriangle(IndexB, vertex, c, lightSources, model);
		}
	}
}

void TFragmentShader::DrawTopFlatTriangle(const TVertex &a, const TVertex &b, const TVertex &c, const vector<TLightSource *> &lightSources, const TModel &model)
{
	if (c.m_vecPoint.m_arrData[1] - a.m_vecPoint.m_arrData[0] == 0 
		|| c.m_vecPoint.m_arrData[1] - b.m_vecPoint.m_arrData[0] == 0
		|| NULL == m_fpDrawLineFunc)
		return;

	// start y,end y
	float fStartY = a.m_vecPoint.m_arrData[1];
	float fEndY = c.m_vecPoint.m_arrData[1];
	int nEndY = (int) fEndY;

	// left vertex,right vertex
	TVertex left, right;
	left.m_vecPoint.m_arrData[1] = fStartY;
	right.m_vecPoint.m_arrData[1] = fStartY;

	for (float i = fStartY; i <= nEndY; i += 1.0f)
	{
		LerpWithY(a, c, left);
		LerpWithY(b, c, right);

		(this->*m_fpDrawLineFunc)(left, right, lightSources, model);

		left.m_vecPoint.m_arrData[1] += 1.0f;
		right.m_vecPoint.m_arrData[1] += 1.0f;
	}
}

void TFragmentShader::DrawDownFlatTriangle(const TVertex &a, const TVertex &b, const TVertex &c, const vector<TLightSource *> &lightSources, const TModel &model)
{
	if (b.m_vecPoint.m_arrData[1] - a.m_vecPoint.m_arrData[1] == 0 
		|| c.m_vecPoint.m_arrData[1] - a.m_vecPoint.m_arrData[1] == 0
		|| NULL == m_fpDrawLineFunc)
		return;

	// start y,end y
	float fStartY = a.m_vecPoint.m_arrData[1];
	float fEndY = c.m_vecPoint.m_arrData[1];
	int nEndY = (int)fEndY;

	// left vertex,right vertex
	TVertex left, right;
	left.m_vecPoint.m_arrData[1] = fStartY;
	right.m_vecPoint.m_arrData[1] = fStartY;

	for (float i = fStartY; i <= nEndY; i += 1.0f)
	{
		LerpWithY(a, b, left);
		LerpWithY(a, c, right);

		(this->*m_fpDrawLineFunc)(left, right, lightSources, model);

		left.m_vecPoint.m_arrData[1] += 1.0f;
		right.m_vecPoint.m_arrData[1] += 1.0f;
	}
}

void TFragmentShader::DrawScanLine(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model)
{
	float fWidth = 1 / (right.m_vecPoint.m_arrData[0] - left.m_vecPoint.m_arrData[0]);
	float fZ = (right.m_vecPoint.m_arrData[2] - left.m_vecPoint.m_arrData[2]) * fWidth;
	float fW = (right.m_vecPoint.m_arrData[3] - left.m_vecPoint.m_arrData[3]) * fWidth;
	float fNx = (right.m_vecN.m_arrData[0] - left.m_vecN.m_arrData[0]) * fWidth;
	float fNy = (right.m_vecN.m_arrData[1] - left.m_vecN.m_arrData[1]) * fWidth;
	float fNz = (right.m_vecN.m_arrData[2] - left.m_vecN.m_arrData[2]) * fWidth;
	float fU = (right.m_nAddress.u - left.m_nAddress.u) * fWidth;
	float fV = (right.m_nAddress.v - left.m_nAddress.v) * fWidth;
	float fR = (right.m_Color.r - left.m_Color.r) * fWidth;
	float fG = (right.m_Color.g - left.m_Color.g) * fWidth;
	float fB = (right.m_Color.b - left.m_Color.b) * fWidth;
	float fAlpha = (right.m_Color.alpha - left.m_Color.alpha) * fWidth;

	int y = left.m_vecPoint.m_arrData[1];
	float fStartZ = left.m_vecPoint.m_arrData[2];
	float fStartW = left.m_vecPoint.m_arrData[3];
	float fStartR = left.m_Color.r;
	float fStartG = left.m_Color.g;
	float fStartB = left.m_Color.b;
	float fStartAlpha = left.m_Color.alpha;
	float fStartU = left.m_nAddress.u;
	float fStartV = left.m_nAddress.v;
	float rhw = 0.0f;
	for (int x = left.m_vecPoint.m_arrData[0]; x <= right.m_vecPoint.m_arrData[0]; x++)
	{
		rhw = 1 / fStartW;
		if (rhw <= m_pDevice->zBuffer[y][x])
		{
			m_pDevice->frameBuffer[y][x] = RGB(fStartR*rhw*255, fStartG*rhw*255, fStartB*rhw*255);
			m_pDevice->zBuffer[y][x] = rhw;
		}
		fStartZ += fZ;
		fStartW += fW;
		fStartR += fR;
		fStartG += fG;
		fStartB += fB;
		fStartAlpha += fAlpha;
		fStartU += fU;
		fStartV += fV;
	}
}

void TFragmentShader::DrawScanLineT(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model)
{
	float fWidth = 1 / (right.m_vecPoint.m_arrData[0] - left.m_vecPoint.m_arrData[0]);
	float fZ = (right.m_vecPoint.m_arrData[2] - left.m_vecPoint.m_arrData[2]) * fWidth;
	float fW = (right.m_vecPoint.m_arrData[3] - left.m_vecPoint.m_arrData[3]) * fWidth;
	float fNx = (right.m_vecN.m_arrData[0] - left.m_vecN.m_arrData[0]) * fWidth;
	float fNy = (right.m_vecN.m_arrData[1] - left.m_vecN.m_arrData[1]) * fWidth;
	float fNz = (right.m_vecN.m_arrData[2] - left.m_vecN.m_arrData[2]) * fWidth;
	float fU = (right.m_nAddress.u - left.m_nAddress.u) * fWidth;
	float fV = (right.m_nAddress.v - left.m_nAddress.v) * fWidth;
	float fR = (right.m_Color.r - left.m_Color.r) * fWidth;
	float fG = (right.m_Color.g - left.m_Color.g) * fWidth;
	float fB = (right.m_Color.b - left.m_Color.b) * fWidth;
	float fAlpha = (right.m_Color.alpha - left.m_Color.alpha) * fWidth;

	int y = left.m_vecPoint.m_arrData[1];
	float fStartZ = left.m_vecPoint.m_arrData[2];
	float fStartW = left.m_vecPoint.m_arrData[3];
	float fStartR = left.m_Color.r;
	float fStartG = left.m_Color.g;
	float fStartB = left.m_Color.b;
	float fStartAlpha = left.m_Color.alpha;
	float fStartU = left.m_nAddress.u;
	float fStartV = left.m_nAddress.v;
	float rhw = 0.0f;
	for (int x = left.m_vecPoint.m_arrData[0]; x <= right.m_vecPoint.m_arrData[0]; x++)
	{
		rhw = 1 / fStartW;
		if (rhw <= m_pDevice->zBuffer[y][x])
		{
			m_pDevice->frameBuffer[y][x] = model.GetTexture(fStartU*rhw, fStartV*rhw);
			m_pDevice->zBuffer[y][x] = rhw;
		}
		fStartZ += fZ;
		fStartW += fW;
		fStartR += fR;
		fStartG += fG;
		fStartB += fB;
		fStartAlpha += fAlpha;
		fStartU += fU;
		fStartV += fV;
	}
}

void TFragmentShader::DrawScanLinePB(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model)
{
	float fWidth = 1 / (right.m_vecPoint.m_arrData[0] - left.m_vecPoint.m_arrData[0]);
	float fZ = (right.m_vecPoint.m_arrData[2] - left.m_vecPoint.m_arrData[2]) * fWidth;
	float fW = (right.m_vecPoint.m_arrData[3] - left.m_vecPoint.m_arrData[3]) * fWidth;
	float fNx = (right.m_vecN.m_arrData[0] - left.m_vecN.m_arrData[0]) * fWidth;
	float fNy = (right.m_vecN.m_arrData[1] - left.m_vecN.m_arrData[1]) * fWidth;
	float fNz = (right.m_vecN.m_arrData[2] - left.m_vecN.m_arrData[2]) * fWidth;
	float fU = (right.m_nAddress.u - left.m_nAddress.u) * fWidth;
	float fV = (right.m_nAddress.v - left.m_nAddress.v) * fWidth;
	float fR = (right.m_Color.r - left.m_Color.r) * fWidth;
	float fG = (right.m_Color.g - left.m_Color.g) * fWidth;
	float fB = (right.m_Color.b - left.m_Color.b) * fWidth;
	float fAlpha = (right.m_Color.alpha - left.m_Color.alpha) * fWidth;

	int y = left.m_vecPoint.m_arrData[1];
	float fStartZ = left.m_vecPoint.m_arrData[2];
	float fStartW = left.m_vecPoint.m_arrData[3];
	float fStartR = left.m_Color.r;
	float fStartG = left.m_Color.g;
	float fStartB = left.m_Color.b;
	float fStartAlpha = left.m_Color.alpha;
	float fStartU = left.m_nAddress.u;
	float fStartV = left.m_nAddress.v;
	float fStartNx = left.m_vecN.m_arrData[0];
	float fStartNy = left.m_vecN.m_arrData[1];
	float fStartNz = left.m_vecN.m_arrData[2];
	float rhw = 0.0f;
	for (int x = left.m_vecPoint.m_arrData[0]; x <= right.m_vecPoint.m_arrData[0]; x++)
	{
		rhw = 1 / fStartW;
		if (rhw <= m_pDevice->zBuffer[y][x])
		{
			TVector point(x, y, fStartZ, fStartW);
			ReverseScreenMapping(point);
			ReversePerspectiveDivision(point);
			TVector normal(fStartNx, fStartNy, fStartNz, 1.0);
			Color result(fStartR, fStartG, fStartB, fStartAlpha);

			vector<TLightSource *>::const_iterator itLight = lightSources.begin();
			for (; itLight != lightSources.end(); itLight++)
			{
				CalcFragmentColorWithLight(point, normal, result, *(*itLight), TVector(0.0f, 0.0f, 0.0f, 1.0f), model.m_Material);
				ModificationsColor(result);
			}

			m_pDevice->frameBuffer[y][x] = RGB(result.r * 255, result.g * 255, result.b * 255);
			m_pDevice->zBuffer[y][x] = rhw;
		}
		fStartZ += fZ;
		fStartW += fW;
		fStartR += fR;
		fStartG += fG;
		fStartB += fB;
		fStartAlpha += fAlpha;
		fStartU += fU;
		fStartV += fV;
		fStartNx += fNx;
		fStartNy += fNy;
		fStartNz += fNz;
	}
}

void TFragmentShader::DrawScanLinePBT(const TVertex &left, const TVertex &right, const vector<TLightSource *> &lightSources, const TModel &model)
{
	float fWidth = 1 / (right.m_vecPoint.m_arrData[0] - left.m_vecPoint.m_arrData[0]);
	float fZ = (right.m_vecPoint.m_arrData[2] - left.m_vecPoint.m_arrData[2]) * fWidth;
	float fW = (right.m_vecPoint.m_arrData[3] - left.m_vecPoint.m_arrData[3]) * fWidth;
	float fNx = (right.m_vecN.m_arrData[0] - left.m_vecN.m_arrData[0]) * fWidth;
	float fNy = (right.m_vecN.m_arrData[1] - left.m_vecN.m_arrData[1]) * fWidth;
	float fNz = (right.m_vecN.m_arrData[2] - left.m_vecN.m_arrData[2]) * fWidth;
	float fU = (right.m_nAddress.u - left.m_nAddress.u) * fWidth;
	float fV = (right.m_nAddress.v - left.m_nAddress.v) * fWidth;
	float fR = (right.m_Color.r - left.m_Color.r) * fWidth;
	float fG = (right.m_Color.g - left.m_Color.g) * fWidth;
	float fB = (right.m_Color.b - left.m_Color.b) * fWidth;
	float fAlpha = (right.m_Color.alpha - left.m_Color.alpha) * fWidth;

	int y = left.m_vecPoint.m_arrData[1];
	float fStartZ = left.m_vecPoint.m_arrData[2];
	float fStartW = left.m_vecPoint.m_arrData[3];
	float fStartR = left.m_Color.r;
	float fStartG = left.m_Color.g;
	float fStartB = left.m_Color.b;
	float fStartAlpha = left.m_Color.alpha;
	float fStartU = left.m_nAddress.u;
	float fStartV = left.m_nAddress.v;
	float rhw = 0.0f;
	for (int x = left.m_vecPoint.m_arrData[0]; x <= right.m_vecPoint.m_arrData[0]; x++)
	{
		rhw = 1 / fStartW;
		if (rhw <= m_pDevice->zBuffer[y][x])
		{
			m_pDevice->frameBuffer[y][x] = model.GetTexture(fStartU*rhw, fStartV*rhw);
			m_pDevice->zBuffer[y][x] = rhw;
		}
		fStartZ += fZ;
		fStartW += fW;
		fStartR += fR;
		fStartG += fG;
		fStartB += fB;
		fStartAlpha += fAlpha;
		fStartU += fU;
		fStartV += fV;
	}
}

void TFragmentShader::SetDevice(Device *pDevice)
{
	if (NULL != pDevice)
		m_pDevice = pDevice;
}

// calc vertex color by light(Blin-Phong Light Model)
void TFragmentShader::CalcFragmentColorWithLight(TVector &point, TVector &normal, Color &result, const TLightSource &LightSource, const TVector &Eye, const Materail &materail)
{
	// l
	TVector LightDir = LightSource.Pos;
	LightDir -= point;
	LightDir.DoNormalize();

	// n
	TVector Normal = normal;
	Normal.DoNormalize();

	// n*l
	float nNL = Normal * LightDir;
	float nDiffuseNL = nNL > 0 ? nNL : 0;

	// DiffuseColor = Clight * Mdiffuse * max(n * l, 0)
	//float index = 1 / (255 * 255);
	float r1 = materail.m_DiffuseColor.r * LightSource.Color.r * nDiffuseNL;
	float g1 = materail.m_DiffuseColor.g * LightSource.Color.g * nDiffuseNL;
	float b1 = materail.m_DiffuseColor.b * LightSource.Color.b * nDiffuseNL;

	// v = eye - point
	TVector v = Eye;
	v -= point;
	v.DoNormalize();

	// h = v + l
	TVector h = v;
	h += LightDir;
	h.DoNormalize();

	// n*h
	float nNH = Normal * h;
	float nSpecularNH = nNH > 0 ? nNH : 0;

	// SpecularColor = Clight * Mspecular * max(n * h, 0)
	float r2 = materail.m_SpecularColor.r * LightSource.Color.r * nSpecularNH;
	float g2 = materail.m_SpecularColor.g * LightSource.Color.g * nSpecularNH;
	float b2 = materail.m_SpecularColor.b * LightSource.Color.b * nSpecularNH;

	result.r = r1 + r2 + LightSource.AmbientColor.r + materail.m_EmessiveColor.r;
	result.g = g1 + g2 + LightSource.AmbientColor.g + materail.m_EmessiveColor.g;
	result.b = b1 + b2 + LightSource.AmbientColor.b + materail.m_EmessiveColor.b;
}

void TFragmentShader::ReverseScreenMapping(TVector &Result)
{
	Result.m_arrData[0] = Result.m_arrData[0] * 2.0f / VIEWWIDTH - 1.0f;
	Result.m_arrData[1] = 1.0f - Result.m_arrData[1] * 2.0f / VIEWHEIGHT;
}

void TFragmentShader::ReversePerspectiveDivision(TVector &Result)
{
	if (0 != Result.m_arrData[3])
	{
		float fDenominator = 1 / (float)Result.m_arrData[3];
		Result.m_arrData[0] *= fDenominator;
		Result.m_arrData[1] *= fDenominator;
		Result.m_arrData[2] *= fDenominator;
		Result.m_arrData[3] = fDenominator;
	}
}