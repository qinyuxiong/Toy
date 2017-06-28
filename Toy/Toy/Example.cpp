#include <Windows.h>
#include <iostream>
#include "Device.h"
#include "Model.h"
#include "TPipeLine.h"

using namespace std;

HINSTANCE hInst;
const TCHAR WndClass[] = "Toy";
const TCHAR WndTitle[] = "Toy";
WNDCLASSEX wcex;


const int CLIP_BOUNDRAY = 1;

//Device *pDevice = NULL;
TPipeLine *pPipeLine = NULL;
//TModel *model = NULL;


WORD RegWndClass(HINSTANCE hInstance);
BOOL InitWnd(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Draw();
void ClearScreen();
void InitDemo();

int nMousePosX = 0;
int nMousePosY = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	if (!RegWndClass(hInstance))
		return false;

	if (!InitWnd(hInstance, nCmdShow))//初始化
		return false;

	InitDemo();

	MSG msg;
	msg.message = WM_NULL;
	while (msg.message != WM_QUIT)//进入消息循环
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

		}
	}
	UnregisterClass(WndClass, wcex.hInstance);
	return msg.wParam;
}

WORD RegWndClass(HINSTANCE hInstance)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_WINICON);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WndClass;
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex);
}

BOOL InitWnd(HINSTANCE hInstance, int nCmdShow)
{
	hwnd = CreateWindow(WndClass, WndTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, VIEWWIDTH + 15, VIEWHEIGHT + 42, NULL, NULL, hInstance, NULL);
	if (!hwnd)
		return false;

	RECT rect = { 0, 0, VIEWWIDTH + 15, VIEWHEIGHT + 42 };
	int wx = rect.right - rect.left;
	int wy = rect.bottom - rect.top;
	int sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	int sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;
	SetWindowPos(hwnd, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_MOUSEMOVE)
	{
		POINT point;
		GetCursorPos(&point);
		if (0 == nMousePosX)
		{
			nMousePosX = point.x;
			nMousePosY = point.y;
		}
		else
		{
			float fRotateX = 0;
			float fRotateY = 0;
			if (point.x > nMousePosX)
				fRotateX = -0.25f;
			else if (point.x < nMousePosX)
				fRotateX = 0.25f;

			if (point.y > nMousePosY)
				fRotateY = 0.25f;
			else if (point.y < nMousePosY)
				fRotateY = -0.25f;

			//pPipeLine->MoveCamera(*(pPipeLine->m_pCamera), fRotateX, fRotateY);
			//Draw();

			nMousePosX = point.x;
			nMousePosY = point.y;
		}

	}
	else if (message == WM_KEYDOWN)
	{
		if (NULL == pPipeLine)
			return 0;

		ClearScreen();
		switch (wParam)
		{
		case VK_UP:
			pPipeLine->RotateCamera(*(pPipeLine->m_pCamera), 0, 0.03);
			break;
		case VK_DOWN:
			pPipeLine->RotateCamera(*(pPipeLine->m_pCamera), 0, -0.03);
			break;
		case VK_RIGHT:
			pPipeLine->RotateCamera(*(pPipeLine->m_pCamera), 0.03, 0);
			break;
		case VK_LEFT:
			pPipeLine->RotateCamera(*(pPipeLine->m_pCamera), -0.03, 0);
			break;
		case 87:
			pPipeLine->MoveCamera(*(pPipeLine->m_pCamera), 0, 1);
			break;
		case 83:
			pPipeLine->MoveCamera(*(pPipeLine->m_pCamera), 0, -1);
			break;
		case 65:
			pPipeLine->MoveCamera(*(pPipeLine->m_pCamera), -1, 0);
			break;
		case 68:
			pPipeLine->MoveCamera(*(pPipeLine->m_pCamera), 1, 0);
			break;
		default:
			break;
		}
		pPipeLine->Run();
		Draw();
	}
	else if (message == WM_LBUTTONDOWN)
	{
		//device->Display(hWnd);
		//pPipeLine->MoveCamera(*(pPipeLine->m_pCamera), 0, 0);
		Draw();
	}
	else if (message == WM_DESTROY)
		PostQuitMessage(0);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitDemo()
{
	pPipeLine = new TPipeLine;
	pPipeLine->InitAll(hwnd, VIEWWIDTH, VIEWHEIGHT);
	//pPipeLine->m_TestModel.ReadModel("teaport.obj");
	//pPipeLine->GetModel("box.obj");
	pPipeLine->AddModel("box.obj");
	pPipeLine->SetMaterial(1, Color(1.0f, 1.0f, 1.0f, 0), Color(0.01f, 0.01f, 0.01f, 0), Color(1.0f, 1.0f, 1.0f, 0));
	//pPipeLine->m_pTestModel->ReadTexture("box.bmp");
	//pPipeLine->GetModel("arcticcondor.obj");

	//pPipeLine->m_TestModel.ReadModel("arakkoa.obj");

	//pPipeLine->m_pReserveModel->SetMaterial(Color(1.0f, 1.0f, 1.0f, 0), Color(0.01f, 0.01f, 0.01f, 0), Color(1.0f, 1.0f, 1.0f, 0));
	pPipeLine->AddLightSource(TVector(0, 3, -7, 0), Color(1, 1, 1, 0), Color(0.01, 0.01, 0.01, 0));
	//pPipeLine->SetState(NORMAL_COLOR_FILL, GROUND_MODEL);
	pPipeLine->SetState(NORMAL_COLOR_FILL, PHONG_BLIN_MODEL/*GROUND_MODEL*/);
	//pPipeLine->SetState(WIRE_FRAME, GROUND_MODEL);
	pPipeLine->Run();
	//pPipeLine->Run(pPipeLine->m_TestModel, WIRE_FRAME);
	//pPipeLine->AddModel("box.obj");
	//model = new TModel;
	//model->ReadTexture("1.bmp");
	//device->CopySurface(model->m_arrTexture, model->m_nTextureHeight, model->m_nTextureWidth);
}

void ThroughVertexShader()
{

}

void Draw()
{
	pPipeLine->m_pDevice->Display(hwnd);
}

void ClearScreen()
{
	pPipeLine->m_pDevice->ClearBuffer();
}