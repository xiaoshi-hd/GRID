#pragma once
class CMyPoint
{
public:
	CMyPoint();
	CMyPoint(CString initPNumb, float initX, float initY, float initZ);
	~CMyPoint();

	CString PNumb;
	float  x, y, z;
	float angle;//与基点的夹角大小
	int nw;//单边交点个数
	double TU;//格网体积

	//在GRIDDlg类中还定义了一个反距离加权法计算高程的函数
};

