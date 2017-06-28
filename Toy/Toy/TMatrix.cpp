#include "TVector.h"
#include "TMatrix.h"
#include <memory>

// default constructor
TMatrix::TMatrix()
{
	DoIdentity();
}

// copy constructor
TMatrix::TMatrix(const TMatrix &matrix)
{
	memcpy(m_arrData, matrix.m_arrData, sizeof(float)* 16);
}

// overload operator =
TMatrix& TMatrix::operator = (const TMatrix &other)
{
	if (this != &other)
		memcpy(m_arrData, other.m_arrData, sizeof(float)* 16);

	return *this;
}

// destructor
TMatrix::~TMatrix()
{
}

// identity matrix
void TMatrix::DoIdentity()
{
	memset(m_arrData, 0, sizeof(int)* 16);
	m_arrData[0][0] = m_arrData[1][1] = m_arrData[2][2] = m_arrData[3][3] = 1.0f;
}

// set all elements to 0
void TMatrix::ZeroAll()
{
	m_arrData[0][0] = m_arrData[0][1] = m_arrData[0][2] = m_arrData[0][3] = 0;
	m_arrData[1][0] = m_arrData[1][1] = m_arrData[1][2] = m_arrData[1][3] = 0;
	m_arrData[2][0] = m_arrData[2][1] = m_arrData[2][2] = m_arrData[2][3] = 0;
	m_arrData[3][0] = m_arrData[3][1] = m_arrData[3][2] = m_arrData[3][3] = 0;
}

void TMatrix::Rotate(float x, float y, float z, float theta)
{
	float qsin = (float)sin(theta);
	float qcos = (float)cos(theta);
	float qcosc = 1 - qcos;
	TVector vec(x, y, z, 1.0f);
	vec.DoNormalize();
	x = vec.m_arrData[0];
	y = vec.m_arrData[1];
	z = vec.m_arrData[2];
	m_arrData[0][0] = x*x*qcosc + qcos;
	m_arrData[0][1] = x*y*qcosc + z*qsin;
	m_arrData[0][2] = x*z*qcosc - y*qsin;
	m_arrData[0][3] = 0.0f;

	m_arrData[1][0] = x*y*qcosc - z*qsin;
	m_arrData[1][1] = y*y*qcosc + qcos;
	m_arrData[1][2] = y*z*qcosc + x*qsin;
	m_arrData[1][3] = 0.0f;

	m_arrData[2][0] = x*z*qcosc + y*qsin;
	m_arrData[2][1] = y*z*qcosc - x*qsin;
	m_arrData[2][2] = z*z*qcosc + qcos;
	m_arrData[2][3] = 0.0f;

	m_arrData[3][0] = 0.0f;
	m_arrData[3][1] = 0.0f;
	m_arrData[3][2] = 0.0f;
	m_arrData[3][3] = 1.0f;
}

// make world transformation matrix
void TMatrix::MakeWorldMatrix(const TVector &vecXCoordinate, const TVector &vecYCoordinate, const TVector &vecZCoordinate, const TVector &vecOrigin)
{
	memcpy(m_arrData[0], vecXCoordinate.m_arrData, sizeof(float)* 4);
	memcpy(m_arrData[1], vecYCoordinate.m_arrData, sizeof(float)* 4);
	memcpy(m_arrData[2], vecZCoordinate.m_arrData, sizeof(float)* 4);
	memcpy(m_arrData[3], vecOrigin.m_arrData, sizeof(float)* 4);
}

// make camera transformation matrix
void TMatrix::MakeCameraMatrix(const TVector &Eye, const TVector &Focus, const TVector &Up)
{
	TVector VectorX = Up;
	TVector VectorZ = Focus;
	VectorZ -= Eye;

	VectorX.DoCrossProduct(VectorZ);

	TVector VectorY = VectorZ;
	VectorY.DoCrossProduct(VectorX);

	VectorX.DoNormalize();
	VectorY.DoNormalize();
	VectorZ.DoNormalize();

	m_arrData[0][0] = VectorX.m_arrData[0];
	m_arrData[0][1] = VectorY.m_arrData[0];
	m_arrData[0][2] = VectorZ.m_arrData[0];
	m_arrData[0][3] = 0;

	m_arrData[1][0] = VectorX.m_arrData[1];
	m_arrData[1][1] = VectorY.m_arrData[1];
	m_arrData[1][2] = VectorZ.m_arrData[1];
	m_arrData[1][3] = 0;

	m_arrData[2][0] = VectorX.m_arrData[2];
	m_arrData[2][1] = VectorY.m_arrData[2];
	m_arrData[2][2] = VectorZ.m_arrData[2];
	m_arrData[2][3] = 0;

	m_arrData[3][0] = -(Eye * VectorX);
	m_arrData[3][1] = -(Eye * VectorY);
	m_arrData[3][2] = -(Eye * VectorZ);
	m_arrData[3][3] = 1;
}

// make Perspective Projection matrix
void TMatrix::MakePerspProMatrix(float Fovy, float Aspect, float ZN, float ZF)
{
	float fax = 1.0f / (float)tan(Fovy * 0.5f);
	ZeroAll();
	m_arrData[0][0] = (float)(fax / Aspect);
	m_arrData[1][1] = (float)(fax);
	m_arrData[2][2] = ZF / (ZF - ZN);
	m_arrData[3][2] = -ZN * ZF / (ZF - ZN);
	m_arrData[2][3] = 1;
}

// make ScreenMapping matrix
void TMatrix::MakeScreenMappingMatrix(int nScreenHeight, int nScreenWidth, int nMaxZ, int nMinZ)
{
	float index1 = nScreenWidth / 2;
	float index2 = nScreenHeight / 2;
	m_arrData[0][0] = index1;
	m_arrData[1][1] = -index2;
	m_arrData[2][2] = nMaxZ - nMinZ;
	m_arrData[3][0] = index1;
	m_arrData[3][1] = index2;
	m_arrData[3][2] = nMinZ;
	m_arrData[3][3] = 1;
}