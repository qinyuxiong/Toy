#ifndef _TVERTEXSHADER_H_
#define _TVERTEXSHADER_H_

#include "TMatrix.h"
#include "utility.h"

class TVertex;

enum TRANS_TYPE
{
	MODEL_TO_WORLD,
	WORLD_TO_CAMERA,
};

class TVertexShader
{
public:
	// default constructor
	TVertexShader();

	// deconstructor
	~TVertexShader();

	// overload operator =
	void operator = (const TVertexShader &VertexShader);

	// copy constructor
	TVertexShader(const TVertexShader &VertexShader);

	// init
	void Init();

	// Set Matrix for model->world transformation
	void SetWorldTrans(const TVector &Origin, const TVector &VectorX, const TVector &VectorY, const TVector &VectorZ);

	// apply model->world transformation
	void ApplyWorldTrans(TVector &Result);

	// Set Matrix for world->camera transformation
	void SetCameraTrans(const TVector &Eye, const TVector &Focus, const TVector &Up);

	// apply world->camera transformation
	void ApplyCameraTrans(TVertex &Result);

	// set Perspective Projection
	void SetPerspectiveProjection(float Fovy, float Aspect, float ZN, float ZF);

	// apply Perspective Projection
	void ApplyPerspectiveProjection(TVertex &Result);

	// perspective division
	void DoPerspectiveDivision(TVertex &Result);

	// calc vertex color by light
	void CalcVertexColorWithLight(TVertex &Result, const TLightSource &LightSource, const TVector &Eye, const Materail &materail);

	void ChangeIntoCVV(TVector &a);

	TMatrix m_WorldMatrix;
	TMatrix m_CameraMatrix;
	TMatrix m_PerProMatrix;
};

#endif