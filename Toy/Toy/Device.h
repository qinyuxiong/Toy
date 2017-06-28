#ifndef _DEVICE_H_
#define _DEVICE_H_
#include <Windows.h>

const int VIEWWIDTH = 1080;
const int VIEWHEIGHT = 720;

static HWND hwnd;

class Device
{
public:
	Device();

	~Device();

	void Init(HWND hWnd);

	void Display(HWND hWnd);

	void CopySurface(unsigned int **pSource, int nHeight, int nWidth);

	void ClearBuffer();

	unsigned int** frameBuffer;
	float **zBuffer;
	HDC hdc;
	HDC hdcMem;
	BITMAPINFO bmpInfo;
	HBITMAP nBmp;
	HBITMAP oBmp;
};

#endif