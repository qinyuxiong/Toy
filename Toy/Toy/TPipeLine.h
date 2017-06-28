#ifndef _TPIPELINE_H_
#define _TPIPELINE_H_

#include "utility.h"
#include "TMatrix.h"
#include "Model.h"

enum RENDER_TYPE
{
	WIRE_FRAME = 1,
	NORMAL_COLOR_FILL,
	COLOR_FILL_WITH_MAP,
};

enum LIGHT_MODEL
{
	GROUND_MODEL = 1,
	PHONG_BLIN_MODEL,
};

class TModel;
class TVertexShader;
class TFragmentShader;
class Device;

class TPipeLine
{
public:
	TPipeLine();

	~TPipeLine();

	TPipeLine(const TPipeLine &pipeLine);

	void Cleanup();

	void operator = (const TPipeLine &pipeLine);

	void SetState(RENDER_TYPE nRenderState, LIGHT_MODEL nLightMode);

	void InitAll(HWND hWnd, int nScreenWidth, int nScreenHeight);

	void InitDevice(HWND hWnd);

	void InitCamera(const TVector &eye, const TVector &focus, const TVector & up);
	
	void InitVertexShader(Camera &camera, int nScreenWidth, int nScreenHeight);

	void InitClip();

	void InitScreenMapping(int nScreenWidth, int nScreenHeight);

	void InitFragmentShader(Device &device);

	void RotateCamera(Camera &camera, float fAngleX, float fAngleY);

	void MoveCamera(Camera &camera, float fOffX, float fOffY);

	void GetModel(const string &strPath);

	void CopyApplyData();

	void Run();

	void GeometryLevel(TModel &model);

	void RasterationLevel(TModel &model);

	void AddLightSource(const TVector &pos, const Color &color, const Color &ambient);

	void AddModel(string strPath);

	void SetMaterial(int nIndex, const Color &a, const Color &b, const Color &c);

	//void ApplyVertexShader();

	BOOL ClipInCVV(const TVector &Result);

	void ScreenMapping(TVector &Result);

	vector<TLightSource *> m_vecTestLightSource;
	vector<TLightSource *> m_vecReserveLightSource;

	vector<TModel *> m_vecTestModels;
	vector<TModel *> m_vecReserveModels;

	TVertexShader *m_pVertexShader;

	TFragmentShader *m_pFragmentShader;

	Device *m_pDevice;

	TMatrix *m_pScreenMap;

	TModel *m_pTestModel;
	TModel *m_pReserveModel;

	Camera *m_pCamera;

	RENDER_TYPE m_nRenderState;

	LIGHT_MODEL m_nLightModel;
};

#endif