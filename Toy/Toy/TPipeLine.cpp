#include "TPipeLine.h"
#include "Device.h"
#include "TVertex.h"
#include "TVertexShader.h"
#include "TFragmentShader.h"
#include <iostream>
#include <strsafe.h>
using namespace std;

const int CLIP_BOUNDRAY = 1;

TPipeLine::TPipeLine()
{
}

TPipeLine::~TPipeLine()
{
	Cleanup();
}

TPipeLine::TPipeLine(const TPipeLine &pipeLine)
{
	if (this != &pipeLine)
		*this = pipeLine;
}

void TPipeLine::Cleanup()
{
	vector<TLightSource *>::iterator it1;
	for (it1 = m_vecTestLightSource.begin(); it1 != m_vecTestLightSource.end(); it1++)
		delete *it1;
	m_vecTestLightSource.clear();

	vector<TLightSource *>::iterator it2;
	for (it2 = m_vecReserveLightSource.begin(); it2 != m_vecReserveLightSource.end(); it2++)
		delete *it2;
	m_vecReserveLightSource.clear();

	delete m_pVertexShader;
	delete m_pFragmentShader;
	delete m_pDevice;
	delete m_pCamera;
	delete m_pScreenMap;
	delete m_pTestModel;
	delete m_pReserveModel;

	//vector<TModel *>::iterator it2;
	//for (it2 = m_vecModels.begin(); it2 != m_vecModels.end(); it2++)
	//	delete *it2;
	//m_vecModels.clear();
}

void TPipeLine::operator = (const TPipeLine &pipeLine)
{
	vector<TLightSource *>::const_iterator it1;
	for (it1 = pipeLine.m_vecTestLightSource.begin(); it1 != pipeLine.m_vecTestLightSource.end(); it1++)
		m_vecTestLightSource.push_back(*it1);

	//vector<TModel *>::const_iterator it2;
	//for (it2 = pipeLine.m_vecModels.begin(); it2 != pipeLine.m_vecModels.end(); it2++)
	//	m_vecModels.push_back(*it2);
}

void TPipeLine::SetState(RENDER_TYPE nRenderState, LIGHT_MODEL nLightMode)
{
	m_nRenderState = nRenderState;
	m_nLightModel = nLightMode;
}

void TPipeLine::InitAll(HWND hWnd, int nScreenWidth, int nScreenHeight)
{
	m_pVertexShader = new TVertexShader;
	m_pFragmentShader = new TFragmentShader;
	m_pDevice = new Device;
	m_pCamera = new Camera;
	m_pScreenMap = new TMatrix;
	m_pTestModel = new TModel;
	m_pReserveModel = new TModel;

	InitDevice(hWnd);

	TVector eye(0, 0, -7, 1);
	TVector focus(0, 0, 0, 1);
	TVector up(0, 1, 0, 1);
	InitCamera(eye, focus, up);

	InitVertexShader(*m_pCamera, nScreenWidth, nScreenHeight);

	InitClip();

	InitScreenMapping(nScreenWidth, nScreenHeight);

	InitFragmentShader(*m_pDevice);
}

void TPipeLine::InitDevice(HWND hWnd)
{
	if (NULL != m_pDevice)
	{
		m_pDevice->Init(hWnd);
		m_pDevice->ClearBuffer();
	}
}

void TPipeLine::InitCamera(const TVector &eye, const TVector &focus, const TVector & up)
{
	if (NULL != m_pCamera)
	{
		m_pCamera->eye = eye;
		m_pCamera->focus = focus;
		m_pCamera->up = up;
	}
}

void TPipeLine::InitVertexShader(Camera &camera, int nScreenWidth, int nScreenHeight)
{
	if (NULL != m_pVertexShader && 0 != nScreenHeight)
	{
		m_pVertexShader->SetCameraTrans(camera.eye, camera.focus, camera.up);

		m_pVertexShader->SetPerspectiveProjection(Pi * 0.5, (float)nScreenWidth /nScreenHeight, 1.0f, 1000.0f);
	}
}

void TPipeLine::InitClip()
{
}

void TPipeLine::InitScreenMapping(int nScreenWidth, int nScreenHeight)
{
	if (NULL != m_pScreenMap)
		m_pScreenMap->MakeScreenMappingMatrix(nScreenHeight, nScreenWidth, 1, 0);
}

void TPipeLine::InitFragmentShader(Device &device)
{
	if (NULL != m_pFragmentShader)
		m_pFragmentShader->SetDevice(&device);
}

void TPipeLine::RotateCamera(Camera &camera, float fAngleX, float fAngleY)
{
	if (NULL != m_pVertexShader)
	{
		TVector VectorZ = camera.focus;
		VectorZ -= camera.eye;

		TMatrix *pMatrix = new TMatrix;
		pMatrix->Rotate(camera.up.m_arrData[0], camera.up.m_arrData[1], camera.up.m_arrData[2], fAngleX);

		VectorZ *= *pMatrix;
		TVector VectorZIndex = VectorZ;
		VectorZ += camera.eye;
		camera.focus = VectorZ;

		TVector VectorX = camera.up;
		VectorX.DoCrossProduct(VectorZIndex);
		pMatrix->Rotate(VectorX.m_arrData[0], VectorX.m_arrData[1], VectorX.m_arrData[2], fAngleY);

		VectorZIndex *= *pMatrix;
		VectorZIndex += camera.eye;
		camera.focus = VectorZIndex;

		VectorZ = camera.focus;
		VectorZ -= camera.eye;

		TVector VectorY = VectorZ;
		VectorY.DoCrossProduct(VectorX);
		camera.up = VectorY;

		camera.up.DoNormalize();
		m_pVertexShader->SetCameraTrans(camera.eye, camera.focus, camera.up);

		delete pMatrix;
	}
}

void TPipeLine::MoveCamera(Camera &camera, float fOffX, float fOffY)
{
	camera.eye.m_arrData[0] += fOffX;
	camera.eye.m_arrData[1] += fOffY;

	camera.focus.m_arrData[0] += fOffX;
	camera.focus.m_arrData[1] += fOffY;

	m_pVertexShader->SetCameraTrans(camera.eye, camera.focus, camera.up);
}

void TPipeLine::GetModel(const string &strPath)
{
	m_pReserveModel->ReadModel(strPath);
}

void TPipeLine::CopyApplyData()
{
	// copy LightSource
	vector<TLightSource *>::iterator it;
	for (it = m_vecTestLightSource.begin(); it != m_vecTestLightSource.end(); it++)
		delete *it;
	m_vecTestLightSource.clear();

	TLightSource *pLight = NULL;
	vector<TLightSource *>::iterator itLight = m_vecReserveLightSource.begin();
	for (; itLight != m_vecReserveLightSource.end(); itLight++)
	{
		pLight = new TLightSource;
		*pLight = *(*itLight);
		m_vecTestLightSource.push_back(pLight);
	}

	// copy Model
	vector<TModel *>::iterator it2;
	for (it2 = m_vecTestModels.begin(); it2 != m_vecTestModels.end(); it2++)
		delete *it2;
	m_vecTestModels.clear();

	TModel *pModel = NULL;
	vector<TModel *>::iterator itModel = m_vecReserveModels.begin();
	for (; itModel != m_vecReserveModels.end(); itModel++)
	{
		pModel = new TModel;
		*pModel = *(*itModel);
		m_vecTestModels.push_back(pModel);
	}
}

void TPipeLine::Run()
{
	CopyApplyData();

	vector<TModel *>::iterator it = m_vecTestModels.begin();
	for (; it != m_vecTestModels.end(); it++)
	{
		GeometryLevel(*(*it));

		RasterationLevel(*(*it));
	}
}

void TPipeLine::GeometryLevel(TModel &model)
{
	// transform the light source to cvv space
	vector<TLightSource *>::iterator itLight = m_vecTestLightSource.begin();
	for (; itLight != m_vecTestLightSource.end(); itLight++)
		m_pVertexShader->ChangeIntoCVV((*itLight)->Pos);

	// deal with each vertex
	vector<TVertex *>::iterator it = model.m_vecVertex.begin();
	TVector Eye(0.0f, 0.0f, 0.0f, 1.0f);
	TVector *pPos = NULL;
	TVertex *pVertex = NULL;
	for (; it != model.m_vecVertex.end(); it++)
	{
		pVertex = *it;
		pPos = &(pVertex->m_vecPoint);

		m_pVertexShader->ApplyCameraTrans(*pVertex);

		m_pVertexShader->ApplyPerspectiveProjection(*pVertex);

		if (FALSE == ClipInCVV(*pPos))
		{
			it = model.m_vecVertex.erase(it);
			continue;
		}

		// set camera¡¢light source and object in the same coordinate
		if (WIRE_FRAME != m_nRenderState && GROUND_MODEL == m_nLightModel)
		{
			vector<TLightSource *>::iterator itLight = m_vecTestLightSource.begin();
			for (; itLight != m_vecTestLightSource.end(); itLight++)
			{
				m_pVertexShader->CalcVertexColorWithLight(*pVertex, *(*itLight), Eye, model.m_Material);
				ModificationsColor((*pVertex).m_Color);
			}
		}

		m_pVertexShader->DoPerspectiveDivision(*pVertex);

		ScreenMapping(*pPos);
	}
}

void TPipeLine::RasterationLevel(TModel &model)
{
	int nSize = model.m_vecVertex.size();
	if (0 != nSize % 3 || 0 == nSize)
		return;

	vector<TVertex*>::iterator a = model.m_vecVertex.begin();
	vector<TVertex*>::iterator b = model.m_vecVertex.begin() + 1;
	vector<TVertex*>::iterator c = model.m_vecVertex.begin() + 2;

	while (1)
	{
		m_pFragmentShader->SortVertex(*(*a), *(*b), *(*c));
		m_pFragmentShader->TraverseTriangle(*(*a), *(*b), *(*c), m_vecTestLightSource, model, m_nRenderState, m_nLightModel);

		if (c + 1 != model.m_vecVertex.end() && c + 2 != model.m_vecVertex.end() && c + 3 != model.m_vecVertex.end())
		{
			a += 3;
			b += 3;
			c += 3;
		}
		else
			break;
	}
}

void TPipeLine::AddLightSource(const TVector &pos, const Color &color, const Color &ambient)
{
	TLightSource *pLightSource = new TLightSource;
	pLightSource->Color = color;
	pLightSource->Pos = pos;
	pLightSource->AmbientColor = ambient;
	m_vecReserveLightSource.push_back(pLightSource);
}

void TPipeLine::AddModel(string strPath)
{
	TModel *pModel = new TModel;
	pModel->ReadModel(strPath);
	m_vecReserveModels.push_back(pModel);
}

void TPipeLine::SetMaterial(int nIndex, const Color &a, const Color &b, const Color &c)
{
	int i = 1;
	vector<TModel *>::iterator it = m_vecReserveModels.begin();
	for (; it != m_vecReserveModels.end(); it++)
	{
		if (nIndex == i)
		{
			(*it)->SetMaterial(a, b, c);
			break;
		}
	}
}

// clip in cvv
BOOL TPipeLine::ClipInCVV(const TVector &Result)
{
	float index = Result.m_arrData[3];
	return (Result.m_arrData[0] >= -index && Result.m_arrData[0] <= index)
		&& (Result.m_arrData[1] >= -index && Result.m_arrData[1] <= index)
		&& (Result.m_arrData[2] >= 0 && Result.m_arrData[2] <= index);
}

//void TPipeLine::ApplyVertexShader()
//{
//	vector<TModel *>::iterator it = m_vecModels.begin();
//	for (; it != m_vecModels.end(); it++)
//	{
//		vector<TVertex*>::iterator it2 = (*it)->m_vecVertex.begin();
//		for (; it2 != (*it)->m_vecVertex.end(); it2++)
//		{
//			m_pVertexShader->ApplyCameraTrans((*it2)->m_vecPoint);
//			m_pVertexShader->ApplyPerspectiveProjection((*it2)->m_vecPoint);
//		}
//	}
//}

void TPipeLine::ScreenMapping(TVector &Result)
{
	Result.m_arrData[0] = (Result.m_arrData[0] + 1.0f)*VIEWWIDTH*0.5f;
	Result.m_arrData[1] = (1.0f - Result.m_arrData[1])*VIEWHEIGHT*0.5f;
}

