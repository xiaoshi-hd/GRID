
// GRIDDlg.h: 头文件
//

#pragma once
#include<stdio.h>
#include<vector>
#include <iostream>
#include "math.h"//用到数学函数

// CGRIDDlg 对话框
class CGRIDDlg : public CDialogEx
{
// 构造
public:
	CGRIDDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRID_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//三个智能指针
	_ConnectionPtr m_pConnection;//连接access数据库的链接对象
	_RecordsetPtr m_pRecordset;//结果集对象
	_CommandPtr m_pCommand;//一个命令对象，可以使用SQL语句

	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnMap();
	afx_msg void OnBnClickedBtnResult();
	afx_msg void OnBnClickedBtnSave();
	CListCtrl m_pointsList;
	afx_msg void OnBnClickedBtnCaulate();
private:
	const float pi = 3.141593;//单精度浮点数精确到小数点后6位，双精度浮点数精确到后15位
	double H0, bian, linyu;//存储基准高程，格网边长，邻域比（在init函数初始化）
	double R0, V, V1, V2;//存储总体积，填方，挖方

public:
	static float max_x, max_y;//存储最大最小的x.y坐标
	static float min_x, min_y;
	static int n_width;//存储格网个数
	static int n_height;

private:
	double gaocheng(double x, double y, double r, vector<CMyPoint> M);//反距离加权法计算格网高程
};
