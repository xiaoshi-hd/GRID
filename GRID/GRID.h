
// GRID.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once
#define BIAN 2  //定义格网边长
#define H_0 9  //基准高程
#define LINYU 0.4  //邻域比

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include < vector> 
#include "CMyPoint.h"
using namespace std;

// CGRIDApp:
// 有关此类的实现，请参阅 GRID.cpp
//

class CGRIDApp : public CWinApp
{
public:
	CGRIDApp();

// 重写
public:
	virtual BOOL InitInstance();

	vector<CMyPoint> vec_P1;//存储导入的点数据(坐标系为测量坐标系)
	vector<CMyPoint> vec_BBPoint;//存放凸闭包上的点(测量坐标)
	static CString report;//计算报告

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CGRIDApp theApp;
