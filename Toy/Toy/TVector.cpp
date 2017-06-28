#include "TVector.h"
#include "TMatrix.h"
#include <memory>

// default constructor
TVector::TVector()
{
	DoIdentity();
}

// copy constructor
TVector::TVector(const TVector &vector)
{
	memcpy(m_arrData, vector.m_arrData, sizeof(float)* 4);
}

// init list constructor
TVector::TVector(float x, float y, float z, float w)
{
	m_arrData[0] = x;
	m_arrData[1] = y;
	m_arrData[2] = z;
	m_arrData[3] = w;
}

// overload operator =
TVector& TVector::operator = (const TVector &vector)
{
	if (this != &vector)
		memcpy(m_arrData, vector.m_arrData, sizeof(float)* 4);
	return *this;
}

// destructor
TVector::~TVector()
{
}

// identity vector
void TVector::DoIdentity()
{
	m_arrData[0] = m_arrData[1] = m_arrData[2] = m_arrData[3] = 1.0f;
}

// normalize vector
void TVector::DoNormalize()
{
	float fDenominator = sqrt(m_arrData[0] * m_arrData[0] + m_arrData[1] * m_arrData[1] + m_arrData[2] * m_arrData[2]);

	if (fDenominator >= -0.0001 && fDenominator <= 0.0001)
		return;
	else
	{
		float index = 1 / fDenominator;
		m_arrData[0] *= index;
		m_arrData[1] *= index;
		m_arrData[2] *= index;
		m_arrData[3] = 1.0f;
	}
}

// cross product
void TVector::DoCrossProduct(const TVector &Vector)
{
	float x = m_arrData[1] * Vector.m_arrData[2] - Vector.m_arrData[1] * m_arrData[2];
	float y = m_arrData[2] * Vector.m_arrData[0] - Vector.m_arrData[2] * m_arrData[0];
	float z = m_arrData[0] * Vector.m_arrData[1] - Vector.m_arrData[0] * m_arrData[1];
	m_arrData[0] = x;
	m_arrData[1] = y;
	m_arrData[2] = z;
	m_arrData[3] = 1;
}

//// overload operator *(matrix)
//TVector TVector::operator * (const TMatrix &matrix)
//{
//    TVector result = *this;
//    result *= matrix;
//    return TVector;
//}

//// overload operator *(int)
//TVector TVector::operator * (int n)
//{
//    TVector result = *this;
//    result *= n;
//    return TVector;
//}

//// overload operator +
//TVector TVector::operator + (const TVector &Vector)
//{
//    TVector result = *this;
//    result += Vector;
//    return TVector;
//}

//// overload operator -
//TVector TVector::operator - (const TVector &Vector)
//{
//    TVector result = *this;
//    result -= Vector;
//    return TVector;
//}

// overload operator *=(matrix)
void TVector::operator *= (const TMatrix &Matrix)
{
	float fResult[4] = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			fResult[i] += m_arrData[j] * Matrix.m_arrData[j][i];
		}
	}

	memcpy(m_arrData, fResult, sizeof(float)* 4);
}

// overload operator *=(int)
void TVector::operator *= (float n)
{
	m_arrData[0] *= n;
	m_arrData[1] *= n;
	m_arrData[2] *= n;
	m_arrData[3] = 1.0f;
}

// overload operator +=
void TVector::operator += (const TVector &Vector)
{
	m_arrData[0] += Vector.m_arrData[0];
	m_arrData[1] += Vector.m_arrData[1];
	m_arrData[2] += Vector.m_arrData[2];
	m_arrData[3] = 1.0f;
}

// overload operator -=
void TVector::operator -= (const TVector &Vector)
{
	m_arrData[0] -= Vector.m_arrData[0];
	m_arrData[1] -= Vector.m_arrData[1];
	m_arrData[2] -= Vector.m_arrData[2];
	m_arrData[3] = 1.0f;
}

// set zero
void TVector::Zero()
{
	m_arrData[0] = 0;
	m_arrData[1] = 0;
	m_arrData[2] = 0;
	m_arrData[3] = 1.0f;
}

// overload operator *
float TVector::operator * (const TVector &Vector) const
{
	return m_arrData[0] * Vector.m_arrData[0] + m_arrData[1] * Vector.m_arrData[1] + m_arrData[2] * Vector.m_arrData[2];
}
