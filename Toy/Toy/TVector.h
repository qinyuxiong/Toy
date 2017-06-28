#ifndef _TVECTOR_H_
#define _TVECTOR_H_

class TMatrix;

class TVector
{
public:
	// default constructor
	TVector();

	// copy constructor
	TVector(const TVector &vector);

	// init list constructor
	TVector(float x, float y, float z, float w);

	// overload operator =
	TVector& operator = (const TVector &vector);

	// destructor
	~TVector();

	// identity vector
	void DoIdentity();

	// normalize vector
	void DoNormalize();

	// cross product
	void DoCrossProduct(const TVector &Vector);

	//// overload operator *(matrix)
	//TVector TVector operator * (const TMatrix &matrix);

	//// overload operator *(int)
	//TVector TVector operator * (int n);

	//// overload operator +
	//TVector TVector operator + (const TVector &Vector);

	//// overload operator -
	//TVector TVector operator - (const TVector &Vector);

	// overload operator *=(matrix)
	void operator *= (const TMatrix &Matrix);

	// overload operator *=(int)
	void operator *= (float n);

	// overload operator +=
	void operator += (const TVector &Vector);

	// overload operator -=
	void operator -= (const TVector &Vector);

	void Zero();

	// overload operator *
	float operator * (const TVector &Vector) const;

	float m_arrData[4];
};

#endif