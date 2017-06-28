#ifndef _TMATRIX_H_
#define _TMATRIX_H_

class TVector;

class TMatrix
{
public:
	// default constructor
	TMatrix();

	// copy constructor
	TMatrix(const TMatrix &matrix);

	// overload operator =
	TMatrix& operator = (const TMatrix &matrix);

	// destructor
	~TMatrix();

	// identity matrix
	void DoIdentity();

	// set all elements to 0
	void ZeroAll();

	// rotate matrix
	void Rotate(float x, float y, float z, float theta);

	// make world transformation matrix
	void MakeWorldMatrix(const TVector &vecXCoordinate, const TVector &vecYCoordinate, const TVector &vecZCoordinate, const TVector &vecOrigin);

	// make camera transformation matrix
	void MakeCameraMatrix(const TVector &Eye, const TVector &Focus, const TVector &Up);

	// make Perspective Projection matrix
	void MakePerspProMatrix(float Fovy, float Aspect, float ZN, float ZF);

	// make ScreenMapping matrix
	void MakeScreenMappingMatrix(int nScreenHeight, int nScreenWidth, int nMaxZ, int nMinZ);

	float m_arrData[4][4];
};

#endif
