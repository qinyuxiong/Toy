#include "Device.h"

Device::Device()
{
	frameBuffer = NULL;
	zBuffer = NULL;
}

Device::~Device()
{
	for (int i = 0; i < VIEWHEIGHT; i++)
	{
		delete frameBuffer[i];
		delete zBuffer[i];
	}
	delete frameBuffer;
	delete zBuffer;
	frameBuffer = NULL;
	zBuffer = NULL;
}

void Device::Display(HWND hWnd)
{
	hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, VIEWWIDTH, VIEWHEIGHT, hdcMem, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
	/*for (int j = 0; j < VIEWHEIGHT; j++)
	{
		memset(frameBuffer[j], 0, VIEWWIDTH*sizeof(unsigned int));
	}*/
}

void Device::Init(HWND hWnd)
{
	hdc = GetDC(hWnd);
	hdcMem = CreateCompatibleDC(hdc);
	ReleaseDC(hWnd, hdc);//暂时断开
	bmpInfo = { { sizeof(BITMAPINFOHEADER), VIEWWIDTH, -VIEWHEIGHT, 1, 32, BI_RGB, VIEWWIDTH * VIEWHEIGHT * 4, 0, 0, 0, 0 } };
	LPVOID ptr;
	nBmp = CreateDIBSection(hdcMem, &bmpInfo, DIB_RGB_COLORS, &ptr, 0, 0);
	oBmp = (HBITMAP)SelectObject(hdcMem, nBmp);

	//frameBuffer每个元素存放指向framebuf=ptr每行的指针
	frameBuffer = new unsigned int *[VIEWHEIGHT];
	zBuffer = new float *[VIEWHEIGHT];

	char*framebuf = (char*)ptr;
	for (int j = 0; j < VIEWHEIGHT; j++)
	{
		frameBuffer[j] = (unsigned int*)(framebuf + VIEWWIDTH * 4 * j);
		zBuffer[j] = new float[VIEWWIDTH];
	}
	int i, j;
}

void Device::CopySurface(unsigned int **pSource, int nHeight, int nWidth)
{
	if (NULL != pSource && nHeight <= VIEWHEIGHT && nWidth <= VIEWWIDTH)
	{
		for (int i = 0; i < nHeight; i++)
		{
			memcpy(frameBuffer[i], pSource[i], sizeof(unsigned int)* nWidth);
		}
	}
}

void Device::ClearBuffer()
{
	for (int i = 0; i < VIEWHEIGHT; i++)
	{
		for (int j = 0; j < VIEWWIDTH; j++)
		{
			frameBuffer[i][j] = 0;
			zBuffer[i][j] = 99999.0f;
		}
	}
}