#pragma once
#include "stdafx.h"
#include "Define.h"

#define SCALE					0
#define TRANSLATE				1
#define ROTATE					2
#define SHEAR					3
#define REFLECT					4
#define NORMAL					5
void TransformAndDraw(int iTransform, HDC _hDC)
{
	HDC hDC;
	XFORM xForm;
	RECT rect;

	// Retrieve a DC handle for the application's window.  

	hDC = _hDC;

	// Set the mapping mode to LOENGLISH. This moves the  
	// client area origin from the upper left corner of the  
	// window to the lower left corner (this also reorients  
	// the y-axis so that drawing operations occur in a true  
	// Cartesian space). It guarantees portability so that  
	// the object drawn retains its dimensions on any display.  

	SetGraphicsMode(hDC, GM_ADVANCED);
	SetMapMode(hDC, MM_LOENGLISH);

	// Set the appropriate world transformation (based on the  
	// user's menu selection).  

	switch (iTransform)
	{
	case SCALE:        // Scale to 1/2 of the original size.  
		xForm.eM11 = (FLOAT) 0.5;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) 0.5;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case TRANSLATE:   // Translate right by 3/4 inch.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) 1.0;
		xForm.eDx = (FLOAT) 75.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case ROTATE:      // Rotate 180 degrees counterclockwise.  
		xForm.eM11 = (FLOAT) -1;
		xForm.eM12 = (FLOAT) 0;
		xForm.eM21 = (FLOAT) 0;
		xForm.eM22 = (FLOAT) 1;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		//ModifyWorldTransform(hDC, &xForm, MWT_LEFTMULTIPLY);
		break;

	case SHEAR:       // Shear along the x-axis with a  
					  // proportionality constant of 1.0.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 1.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) 1.0;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case REFLECT:     // Reflect about a horizontal axis.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT)-1.0;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;

	case NORMAL:      // Set the unity transformation.  
		xForm.eM11 = (FLOAT) 1.0;
		xForm.eM12 = (FLOAT) 0.0;
		xForm.eM21 = (FLOAT) 0.0;
		xForm.eM22 = (FLOAT) 1.0;
		xForm.eDx = (FLOAT) 0.0;
		xForm.eDy = (FLOAT) 0.0;
		SetWorldTransform(hDC, &xForm);
		break;
	}
}
HBITMAP GetRotatedBitmap(HDC hdc, HBITMAP hBitmap, float radians, COLORREF clrBack)
{
	// Create a memory DC compatible with the display
	HDC sourceDC, destDC;
	BITMAP bm;

	HBITMAP hbmResult;
	HBITMAP hbmOldSource;
	HBITMAP hbmOldDest;
	HBRUSH hbrBack;
	HBRUSH hbrOld;

	XFORM xform;

	float cosine, sine;
	int x1, y1, x2, y2, x3, y3, minx, miny, maxx, maxy, w, h;


	sourceDC = CreateCompatibleDC(hdc);
	destDC = CreateCompatibleDC(hdc);



	// Get logical coordinates
	GetObject(hBitmap, sizeof(bm), &bm);

	cosine = cos(radians);
	sine = sin(radians);



	//회전된 이미지의 영역을 구한다.
	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	x1 = (int)(bm.bmHeight * sine);
	y1 = (int)(bm.bmHeight * cosine);
	x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	x3 = (int)(bm.bmWidth * cosine);
	y3 = (int)(-bm.bmWidth * sine);

	minx = min(0, min(x1, min(x2, x3)));
	miny = min(0, min(y1, min(y2, y3)));
	maxx = max(0, max(x1, max(x2, x3)));
	maxy = max(0, max(y1, max(y2, y3)));

	w = maxx - minx;
	h = maxy - miny;



	// Create a bitmap to hold the result
	hbmResult = CreateCompatibleBitmap(hdc, w, h);

	hbmOldSource = (HBITMAP)SelectObject(sourceDC, hBitmap);
	hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult);



	// Draw the background color before we change mapping mode
	hbrBack = CreateSolidBrush(clrBack);
	hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);
	PatBlt(destDC, 0, 0, w, h, PATCOPY);
	DeleteObject(SelectObject(destDC, hbrOld));



	// We will use world transform to rotate the bitmap
	SetGraphicsMode(destDC, GM_ADVANCED);
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;

	SetWorldTransform(destDC, &xform);



	// Now do the actual rotating - a pixel at a time
	BitBlt(destDC, 0, 0, bm.bmWidth, bm.bmHeight, sourceDC, 0, 0, SRCCOPY);



	// Restore DCs
	SelectObject(sourceDC, hbmOldSource);
	SelectObject(destDC, hbmOldDest);

	DeleteDC(sourceDC);
	DeleteDC(destDC);



	return hbmResult;
}


