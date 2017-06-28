#include "TVertexShader.h"
#include "TVector.h"
#include "TVertex.h"
#include "utility.h"

// default constructor
TVertexShader::TVertexShader()
{

}

// deconstructor
TVertexShader::~TVertexShader()
{

}

// overload operator =
void TVertexShader::operator = (const TVertexShader &VertexShader)
{

}

// copy constructor
TVertexShader::TVertexShader(const TVertexShader &VertexShader)
{

}

// init
void TVertexShader::Init()
{

}

// Set Matrix for model->world transformation
void TVertexShader::SetWorldTrans(const TVector &Origin, const TVector &VectorX, const TVector &VectorY, const TVector &VectorZ)
{
	m_WorldMatrix.MakeWorldMatrix(VectorX, VectorY, VectorZ, Origin);
}

// apply model->world transformation
void TVertexShader::ApplyWorldTrans(TVector &Result)
{
	Result *= m_WorldMatrix;
}

// Set Matrix for world->camera transformation
void TVertexShader::SetCameraTrans(const TVector &Eye, const TVector &Focus, const TVector &Up)
{
	m_CameraMatrix.MakeCameraMatrix(Eye, Focus, Up);
}

// apply world->camera transformation
void TVertexShader::ApplyCameraTrans(TVertex &Result)
{
	Result.m_vecPoint *= m_CameraMatrix;
}

// set Perspective Projection
void TVertexShader::SetPerspectiveProjection(float Fovy, float Aspect, float ZN, float ZF)
{
	m_PerProMatrix.MakePerspProMatrix(Fovy, Aspect, ZN, ZF);
}

// apply Perspective Projection
void TVertexShader::ApplyPerspectiveProjection(TVertex &Result)
{
	Result.m_vecPoint *= m_PerProMatrix;
}

// perspective division
void TVertexShader::DoPerspectiveDivision(TVertex &Result)
{
	if (Result.m_vecPoint.m_arrData[3] != 0)
	{
		float fDenominator = 1 / (float)Result.m_vecPoint.m_arrData[3];
		Result.m_vecPoint.m_arrData[0] *= fDenominator;
		Result.m_vecPoint.m_arrData[1] *= fDenominator;
		Result.m_vecPoint.m_arrData[2] *= fDenominator;
		Result.m_vecPoint.m_arrData[3] = fDenominator;
		Result.m_Color.r *= fDenominator;
		Result.m_Color.g *= fDenominator;
		Result.m_Color.b *= fDenominator;;
		Result.m_Color.alpha *= fDenominator;
		Result.m_nAddress.u *= fDenominator;
		Result.m_nAddress.v *= fDenominator;
		Result.m_vecN *= fDenominator;
	}
}

// calc vertex color by light(Blin-Phong Light Model)
void TVertexShader::CalcVertexColorWithLight(TVertex &Result, const TLightSource &LightSource, const TVector &Eye, const Materail &materail)
{
	// l
	TVector LightDir = LightSource.Pos;
	LightDir -= Result.m_vecPoint;
	LightDir.DoNormalize();
	
	// n
	TVector Normal = Result.m_vecN;
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
	v -= Result.m_vecPoint;
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

	Result.m_Color.r = r1 + r2 + LightSource.AmbientColor.r + materail.m_EmessiveColor.r;
	Result.m_Color.g = g1 + g2 + LightSource.AmbientColor.g + materail.m_EmessiveColor.g;
	Result.m_Color.b = b1 + b2 + LightSource.AmbientColor.b + materail.m_EmessiveColor.b;
}

void TVertexShader::ChangeIntoCVV(TVector &a)
{
	a *= m_CameraMatrix;
	a *= m_PerProMatrix;
}

