#include "Model.h"
#include "TVertex.h"
#include "TVector.h"
#include <atlimage.h>
#include <fstream>
using namespace std;

TModel::TModel()
{
	m_arrTexture = NULL;
	m_nTextureHeight = 0;
	m_nTextureWidth = 0;
	m_vecVertex.clear();
}

TModel::~TModel()
{
	Cleanup();
}

TModel::TModel(const TModel &model)
{
	Cleanup();

	m_nTextureHeight = model.m_nTextureHeight;
	m_nTextureWidth = model.m_nTextureWidth;

	m_arrTexture = new unsigned int *[m_nTextureHeight];
	for (int i = 0; i < m_nTextureHeight; i++)
	{
		m_arrTexture[i] = new unsigned int[m_nTextureWidth];
		memcpy(m_arrTexture[i], model.m_arrTexture[i], sizeof (unsigned int)* m_nTextureWidth);
	}

	TVertex *pVertex = NULL;
	vector<TVertex*>::const_iterator it;
	for (it = model.m_vecVertex.begin(); it != model.m_vecVertex.end(); it++)
	{
		pVertex = new TVertex;
		*pVertex = *(*it);
		m_vecVertex.push_back(pVertex);
	}

	m_Material = model.m_Material;
}

TModel& TModel::operator = (const TModel &model)
{
	if (this != &model)
	{
		Cleanup();

		//m_nTextureHeight = model.m_nTextureHeight;
		//m_nTextureWidth = model.m_nTextureWidth;

		//m_arrTexture = new unsigned int *[m_nTextureHeight];
		//for (int i = 0; i < m_nTextureHeight; i++)
		//{
		//	m_arrTexture[i] = new unsigned int[m_nTextureWidth];
		//	memcpy(m_arrTexture[i], model.m_arrTexture[i], sizeof (unsigned int)* m_nTextureWidth);
		//}

		TVertex *pVertex = NULL;
		vector<TVertex*>::const_iterator it;
		for (it = model.m_vecVertex.begin(); it != model.m_vecVertex.end(); it++)
		{
			pVertex = new TVertex;
			*pVertex = *(*it);
			m_vecVertex.push_back(pVertex);
		}

		m_Material = model.m_Material;
	}
	return *this;
}

void TModel::Cleanup()
{
	for (int i = 0; i < m_nTextureHeight; i++)
		delete m_arrTexture[i];
	delete m_arrTexture;
	m_arrTexture = NULL;

	vector<TVertex*>::iterator it;
	for (it = m_vecVertex.begin(); it != m_vecVertex.end(); it++)
		delete *it;

	m_vecVertex.clear();
}

void TModel::ReadTexture(const string &strPath)
{
	CImage image;
	image.Load(strPath.c_str());
	m_nTextureWidth = image.GetWidth();
	m_nTextureHeight = image.GetHeight();
	m_arrTexture = new unsigned int *[m_nTextureHeight];
	for (int i = 0; i < m_nTextureHeight; i++)
	{
		m_arrTexture[i] = new unsigned int[m_nTextureWidth];
	}
	int R, G, B;
	int i, j;
	for (i = 0; i < m_nTextureHeight; i++)
	{
		for (j = 0; j < m_nTextureWidth; j++)
		{
			unsigned int index = image.GetPixel(j, i);
			R = GetRValue(index);
			G = GetGValue(index);
			B = GetBValue(index);
			m_arrTexture[i][j] = RGB(B, G, R);
		}
	}
	image.Destroy();
}

void TModel::ReadModel(const string &strPath)
{
	char szTextLine[255] = { 0 };
	char szWord[255] = { 0 };
	float x, y, z;
	int x1, y1, z1, x2, y2, z2, x3, y3, z3;

	vector<TVector*> vecPos;
	vector<TVector*> vecNormal;
	vector<TVector*> vecTexture;
	TVector *pVector = NULL;
	TVertex *pVertex1 = NULL;
	TVertex *pVertex2 = NULL;
	TVertex *pVertex3 = NULL;
	//PrimitiveIndex *pIndex1 = NULL;
	//PrimitiveIndex *pIndex2 = NULL;
	//PrimitiveIndex *pIndex3 = NULL;

	ifstream file;
	file.open(strPath, std::ios::in);
	if (!file)
		return;

	file.getline(szTextLine, 255);
	while (!file.eof())
	{
		file.getline(szTextLine, 255);
		if (0 == strcmp("" , szTextLine))
			continue;

		if (sscanf_s(szTextLine, "%s %f %f %f", &szWord, sizeof(szWord), &x, &y, &z) == 4)
		{
			if (0 == strcmp("v", szWord))
			{
				pVector = new TVector(x, y, z, 1);
				vecPos.push_back(pVector);
			}
			else if (0 == strcmp("vn", szWord))
			{
				pVector = new TVector(x, y, z, 0);
				vecNormal.push_back(pVector);
			}
			else if (0 == strcmp("vt", szWord))
			{
				pVector = new TVector(x, y, z, 1);
				vecTexture.push_back(pVector);
			}
		}
		else if (sscanf_s(szTextLine, "%s %d/%d/%d %d/%d/%d %d/%d/%d", &szWord, sizeof(szWord), &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3) == 10) // pos, texture, normal
		{
			if (0 == strcmp("f", szWord))
			{
				// copy
				pVertex1 = new TVertex(*vecPos[x1 - 1], TMapAddress(vecTexture[y1 - 1]->m_arrData[0], vecTexture[y1 - 1]->m_arrData[1]), *vecNormal[z1 - 1]);
				pVertex2 = new TVertex(*vecPos[x2 - 1], TMapAddress(vecTexture[y2 - 1]->m_arrData[0], vecTexture[y2 - 1]->m_arrData[1]), *vecNormal[z2 - 1]);
				pVertex3 = new TVertex(*vecPos[x3 - 1], TMapAddress(vecTexture[y3 - 1]->m_arrData[0], vecTexture[y3 - 1]->m_arrData[1]), *vecNormal[z3 - 1]);

				m_vecVertex.push_back(pVertex1);
				m_vecVertex.push_back(pVertex2);
				m_vecVertex.push_back(pVertex3);
				//pIndex1 = new PrimitiveIndex(x1, y1, z1);

				//pIndex2 = new PrimitiveIndex(x2, y2, z2);

				//pIndex3 = new PrimitiveIndex(x3, y3, z3);

				//m_vecIndex.push_back(pIndex1);
				//m_vecIndex.push_back(pIndex1);
				//m_vecIndex.push_back(pIndex1);
			}
		}
	}

	vector<TVector*>::iterator it;
	for (it = vecPos.begin(); it != vecPos.end(); it++)
		delete *it;
	for (it = vecNormal.begin(); it != vecNormal.end(); it++)
		delete *it;
	for (it = vecTexture.begin(); it != vecTexture.end(); it++)
		delete *it;

	file.close();
}

void TModel::SetMaterial(const Color &a, const Color &b, const Color &c)
{
	m_Material.m_DiffuseColor = a;
	m_Material.m_EmessiveColor = b;
	m_Material.m_SpecularColor = c;
}

unsigned int TModel::GetTexture(float x, float y) const
{
	if (1.000 < x || 1.000 < y)
		return 0;

	int nX = (m_nTextureWidth * x) <= 0 ? 0 : m_nTextureHeight * x - 1;
	int nY = (m_nTextureHeight * y) <= 0 ? 0 : m_nTextureHeight * y - 1;
	return m_arrTexture[nY][nX];
}