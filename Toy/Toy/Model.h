#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>
#include <vector>
#include "utility.h"
using namespace std;

typedef struct PIndex
{
	PIndex(int x, int y, int z) :nIndex1(x), nIndex2(y), nIndex3(z){}
	int nIndex1, nIndex2, nIndex3;
} PrimitiveIndex;

class TVertex;

class TModel
{
public:
	TModel();

	~TModel();

	TModel(const TModel &model);

	TModel& operator = (const TModel &model);

	void Cleanup();

	void ReadTexture(const string &strPath);

	void ReadModel(const string &strPath);

	void SetMaterial(const Color &a, const Color &b, const Color &c);

	unsigned int GetTexture(float x, float y) const;

	unsigned int **m_arrTexture;
	int m_nTextureHeight;
	int m_nTextureWidth;
	vector<TVertex*> m_vecVertex;
	Materail m_Material;
	//vector<PrimitiveIndex*> m_vecIndex;
};

#endif