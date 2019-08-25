// CMapDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "GRID.h"
#include "CMapDlg.h"
#include "afxdialogex.h"
#include "GRIDDlg.h"


// CMapDlg 对话框

IMPLEMENT_DYNAMIC(CMapDlg, CDialogEx)

CMapDlg::CMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MAP, pParent)
{

}

CMapDlg::~CMapDlg()
{
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SHOWPOINT, &CMapDlg::OnBnClickedBtnShowpoint)
	ON_BN_CLICKED(IDC_BTN_SHOWBAG, &CMapDlg::OnBnClickedBtnShowbag)
	ON_BN_CLICKED(IDC_BTN_SHOWGRID, &CMapDlg::OnBnClickedBtnShowgrid)
	ON_BN_CLICKED(IDC_BTN_SAVEMAP, &CMapDlg::OnBnClickedBtnSavemap)
END_MESSAGE_MAP()


// CMapDlg 消息处理程序

BOOL CMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CGRIDApp *app = (CGRIDApp *)AfxGetApp(); //生成指向应用程序类的指针

	//根据点坐标调节窗口大小

	multiple_W = (float)(800.0 / (CGRIDDlg::max_x - CGRIDDlg::min_x));//屏幕大小自定义为800，800
	multiple_H = (float)(800.0 / (CGRIDDlg::max_y - CGRIDDlg::min_y));
	Width = (int)((CGRIDDlg::max_x - CGRIDDlg::min_x) * multiple_W);
	Height = (int)((CGRIDDlg::max_y - CGRIDDlg::min_y) * multiple_H);
	CRect temprect(0, 0, Width, Height);
	//设置对话框大小
	SetWindowPos(NULL, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	//对话框居中显示
	CWnd::CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CMapDlg::OnBnClickedBtnShowpoint()//显示点的事件处理函数
{
	// TODO: 在此添加控件通知处理程序代码

	vec_P2.clear();

	CClientDC dc(this);
	CBrush newBrush(RGB(255,0,0));//设置画刷填充为红色
	CBrush *oldBrush;
	oldBrush =  dc.SelectObject(&newBrush);

	CGRIDApp *app = (CGRIDApp*)AfxGetApp();
	CPoint point;
	for (UINT i = 0; i < app->vec_P1.size(); i++)//实现数学坐标到屏幕坐标的坐标转换
	{
		point.x = (app->vec_P1[i].x - CGRIDDlg::min_x)*(multiple_W * 3 / 4) + Width / 8;
		//按3/4的比例缩小，之后就会多出1/4的位置，再在左边加1/8的位置
		//y坐标这里必须用宽度的比例系数，因为屏幕是正方形，但是我们的是的坐标不一定是正方形，要保证缩放比例一致
		point.y = (CGRIDDlg::max_y - app->vec_P1[i].y)*(multiple_W * 3 / 4) + Width / 3;//根据示例数据设置的1/3
		vec_P2.push_back(point);
	}

	for (UINT i = 0; i < vec_P2.size(); ++i)
	{
		//指定绘制（椭圆）圆的指定限定矩形四角的坐标,圆的半径为2
		dc.Ellipse((int)vec_P2[i].x - 2, (int)vec_P2[i].y + 2, (int)vec_P2[i].x + 2, (int)vec_P2[i].y - 2);
	}

	dc.SelectObject(oldBrush);//设置为旧画刷
	newBrush.DeleteObject();//删除新画刷，释放内存
}

void CMapDlg::OnBnClickedBtnShowbag()//显示闭包的事件处理函数
{
	// TODO: 在此添加控件通知处理程序代码

	vec_bbPoint.clear();

	CGRIDApp *app = (CGRIDApp *)AfxGetApp(); //生成指向应用程序类的指针

	CClientDC dc(this);
	//坐标转换
	CPoint point;
	for (UINT i = 1; i < app->vec_BBPoint.size(); i++)
	{
		point.x = (app->vec_BBPoint[i].x - CGRIDDlg::min_x)*(multiple_W * 3 / 4) + Width / 8;
		point.y = (CGRIDDlg::max_y - app->vec_BBPoint[i].y)*(multiple_W * 3 / 4) + Width / 3;
		vec_bbPoint.push_back(point);
	}
	dc.MoveTo(vec_bbPoint[0].x, vec_bbPoint[0].y);//画线
	for (UINT i = 1; i < vec_bbPoint.size(); i++)
	{
		dc.LineTo(vec_bbPoint[i].x, vec_bbPoint[i].y);
	}
	dc.LineTo(vec_bbPoint[0].x, vec_bbPoint[0].y);
}


void CMapDlg::OnBnClickedBtnShowgrid()//显示格网的事件处理函数
{
	// TODO: 在此添加控件通知处理程序代码
	CClientDC dc(this);
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//透明画刷
	CBrush *oldBrush;
	CPen pen(PS_SOLID,1,RGB(0,255,0));
	oldBrush = dc.SelectObject(pBrush);
	dc.SelectObject(&pen);

	/*for (int i = 0; i < CGRIDDlg::n_width; i++)
	{
		for (int j = 0; j < CGRIDDlg::n_height; j++)
		{
			//通过画一个个小矩形窗格来显示网格
			CRect rect;
			rect.left = i * (multiple_W * 3 / 4) + Width / 8;
			rect.right = rect.left + multiple_W * 3 / 4;
			rect.top = j * (multiple_W * 3 / 4) + Width / 3;
			rect.bottom = rect.top + multiple_W * 3 / 4;

			dc.Rectangle(rect);
		}
	}*/

	for (int i = 0; i <= CGRIDDlg::n_width; i++)
	{
		//通过画线来显示网格
		dc.MoveTo(i * BIAN * multiple_W * 3 / 4 + Width / 8, Width / 3);
		dc.LineTo(i * BIAN * multiple_W * 3 / 4 + Width / 8, CGRIDDlg::n_height * BIAN * (multiple_W * 3 / 4) + Width / 3);
	}
	for (int j = 0; j <= CGRIDDlg::n_height; j++)
	{
		dc.MoveTo(Width / 8, j * BIAN * (multiple_W * 3 / 4) + Width / 3);
		dc.LineTo(Width * 7 / 8 + (multiple_W * 3 / 4), j * BIAN * (multiple_W * 3 / 4) + Width / 3);
	}

	dc.SelectObject(oldBrush);//设置为旧画刷
	pBrush->DeleteObject();//删除新画刷，释放内存
}


void CMapDlg::OnBnClickedBtnSavemap()//图形保存
{
	// TODO: 在此添加控件通知处理程序代码
	CClientDC dc(this);//直接获取了屏幕的dc
	CDC m_dcCompatible;
	m_dcCompatible.CreateCompatibleDC(&dc);//创建兼容DC
	CRect rect;
	GetClientRect(&rect);//获取客户区大小
	CBitmap bitmap;
	//由于只想显示对话框的部分内容，所以将兼容位图的高度修改了一下
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), (int)((CGRIDDlg::max_y - CGRIDDlg::min_y) * multiple_W) + Height / 12);//创建兼容位图
	m_dcCompatible.SelectObject(&bitmap);//将位图选入dc
	m_dcCompatible.BitBlt(0, 0, rect.right - rect.left, (int)((CGRIDDlg::max_y - CGRIDDlg::min_y) * multiple_W) + Height / 12, &dc, 0, Height / 4, SRCCOPY);
	//将当前窗口dc的眼色表和像素数据拷贝到兼容dc中

	CImage image;
	image.Attach(bitmap);//绑定一个Bitmap到image中
	CFileDialog fileDlg(FALSE);
	fileDlg.m_ofn.lpstrTitle = "图形保存";//openfilename结构体中有很多参数，修改对应得参数改变对话框的功能
	fileDlg.m_ofn.lpstrFilter = "BITMAP(*.bmp)\0*.bmp\0JPEG(*.jpg)\0*.jpg\0\0";
	fileDlg.m_ofn.lpstrDefExt = "bmp";
	if (IDOK == fileDlg.DoModal())
	{
		HRESULT hResult = image.Save(fileDlg.GetPathName());     //保存图像
		if (FAILED(hResult))
		{
			MessageBox(_T("保存图像文件失败！"));
		}
		else
		{
			MessageBox(_T("文件保存成功！"));
		}
	}
	image.Detach();//分离image对象的位图
}



