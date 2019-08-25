#include "stdafx.h"
#include "CMyPoint.h"


CMyPoint::CMyPoint()
{
}

CMyPoint::CMyPoint(CString initPNumb, float initX, float initY, float initZ)
{
	PNumb = initPNumb;
	x = initX;
	y = initY;
	z = initZ;
}

CMyPoint::~CMyPoint()
{
}


