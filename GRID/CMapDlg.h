#pragma once
#include <vector>

// CMapDlg 对话框

class CMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDlg)

public:
	CMapDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMapDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnShowpoint();
	afx_msg void OnBnClickedBtnShowbag();
	afx_msg void OnBnClickedBtnShowgrid();
	afx_msg void OnBnClickedBtnSavemap();
	virtual BOOL OnInitDialog();
private:
	
	float multiple_W, multiple_H;//存储屏幕大小与实地大小的比例系数
	int Width;//屏幕宽，高
	int Height;

public:
	vector<CPoint> vec_P2;//存储客户区坐标系的点数据
	vector<CPoint> vec_bbPoint;//存储闭包的屏幕坐标

};
