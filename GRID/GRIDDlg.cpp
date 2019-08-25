
// GRIDDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "GRID.h"
#include "GRIDDlg.h"
#include "afxdialogex.h"
#include "CMyPoint.h"
#include "CMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGRIDDlg 对话框



CGRIDDlg::CGRIDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRID_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGRIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_pointsList);
}

BEGIN_MESSAGE_MAP(CGRIDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CGRIDDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_MAP, &CGRIDDlg::OnBnClickedBtnMap)
	ON_BN_CLICKED(IDC_BTN_RESULT, &CGRIDDlg::OnBnClickedBtnResult)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CGRIDDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CAULATE, &CGRIDDlg::OnBnClickedBtnCaulate)
END_MESSAGE_MAP()


// CGRIDDlg 消息处理程序
float CGRIDDlg::max_x = 0;//初始化静态变量
float CGRIDDlg::max_y = 0;
float CGRIDDlg::min_x = 0;
float CGRIDDlg::min_y = 0;
int CGRIDDlg::n_width = 0;
int CGRIDDlg::n_height = 0;

BOOL CGRIDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	H0 = H_0;//基准高程
	bian = BIAN;//网格边长
	linyu = LINYU;//邻域比

	CRect rect;
	// 获取列表视图控件的位置和大小   
	m_pointsList.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_pointsList.SetExtendedStyle(m_pointsList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加四列   
	m_pointsList.InsertColumn(0, _T("点名"), LVCFMT_CENTER, rect.Width() / 4, 0);
	m_pointsList.InsertColumn(1, _T("X坐标"), LVCFMT_CENTER, rect.Width() / 4, 1);
	m_pointsList.InsertColumn(2, _T("Y坐标"), LVCFMT_CENTER, rect.Width() / 4, 2);
	m_pointsList.InsertColumn(3, _T("Z坐标"), LVCFMT_CENTER, rect.Width() / 4, 3);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGRIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGRIDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGRIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGRIDDlg::OnBnClickedBtnOpen()//文件打开的事件处理程序
{
	// TODO: 在此添加控件通知处理程序代码

	CGRIDApp *app = (CGRIDApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->vec_P1.clear();//清除列表中的数据
	m_pointsList.DeleteAllItems();//删除所有显示的数据

	CFileDialog fileDlg(TRUE);//参数为TRUE表示文件打开，FALSE表示文件保存
	fileDlg.m_ofn.lpstrTitle = "文件打开";//openfilename结构体中有很多参数，修改对应得参数改变对话框的功能
	fileDlg.m_ofn.lpstrFilter = "Text Files(*.txt)\0*.txt\0Database Files(*.mdb)\0*.mdb\0All Files(*.*)\0*.*\0\0";
	//设置文件类型过滤器，每写一个字符串加一个\0,最后两个\0
	fileDlg.m_ofn.lpstrDefExt = "txt";//默认打开类型
	CMyPoint point;

	if (IDOK == fileDlg.DoModal())
	{
#pragma region 1.txt文件打开
		if ("txt" == fileDlg.GetFileExt())
		{
			CStdioFile file(fileDlg.GetPathName(), CFile::modeRead | CFile::typeText);
			CString strLine, strTemp, result;
			int row = 0;

			try
			{
				while (file.ReadString(strLine))//每次读取一行
				{
					/*int count = strLine.Replace(',',' ');//将逗号替换为空格，主要是为了返回值逗号的数目
					if (0 == count)
					{
						MessageBox("文件没有数据！");
						return;
					}
					strLine.Replace(' ', ',');*/

					int n = strLine.Find(",");//返回第一次出现的位置（索引位置，从0开始），如果没有，返回-1
					strTemp = strLine.Left(n);
					point.PNumb = strTemp;
					strLine = strLine.Right(strLine.GetLength() - n - 1);//去掉第一个逗号及之前的内容
					n = strLine.Find(",");
					strLine = strLine.Right(strLine.GetLength() - n - 1);//把第二个没数据的逗号去掉
					n = strLine.Find(",");

					float* num = new float[3];//存储坐标值
					int i = 0;
					while (-1 != n)
					{
						strTemp = strLine.Left(n);
						num[i] = (float)atof(strTemp);
						i++;
						strLine = strLine.Right(strLine.GetLength() - n - 1);
						n = strLine.Find(",");
					}
					if (strLine.GetLength() > 0)//最后一个数据
					{
						num[i] = (float)atof(strLine);
					}
					point.x = num[0];
					point.y = num[1];
					point.z = num[2];
					app->vec_P1.push_back(point);//为动态数组赋值
					row++;
				}
			}
			catch (...)//捕获所有异常
			{
				AfxMessageBox("请检查文本文件中的数据是否正确！\n数据导入失败！");
				return;
			}
			file.Close();

		}
#pragma endregion

#pragma region 2.数据库mdb文件导入
		if ("mdb" == fileDlg.GetFileExt())
		{
			CString path = fileDlg.GetPathName();
			AfxOleInit();//初始化COM组件，并在结束时自动关闭

			m_pConnection.CreateInstance(__uuidof(Connection));//创建链接对象

			try
			{
				m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source="+(_bstr_t)path, "", "", adModeUnknown);//打开本地Access库
				 //如果文档不存在则抛出CFileException异常  
			}
			catch(_com_error e)
			{
				CString errormessage;
				errormessage.Format(_T("连接数据库失败!/n错误信息:%s"), e.ErrorMessage());
				AfxMessageBox(errormessage);
				return;
			}
			AfxMessageBox("成功打开数据库!");

			m_pRecordset.CreateInstance(__uuidof(Recordset));//创建数据表

			try
			{
				m_pRecordset->Open("SELECT * FROM points", m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
				//打开一个记录集（points表），通过这个记录我们可以进行查询，插入，删除等操作。
			}
			catch (_com_error *e)//捕获COM抛出的异常
			{
				AfxMessageBox(e->ErrorMessage());
				return;
			}

			try
			{
				while (!m_pRecordset->EndOfFile)//判断是否移动到数据表末尾
				{
					point.PNumb = m_pRecordset->GetCollect("PNumb");
					point.x = m_pRecordset->GetCollect("X");
					point.y = m_pRecordset->GetCollect("Y");
					point.z = m_pRecordset->GetCollect("Z");
					app->vec_P1.push_back(point);//为动态数组赋值
					m_pRecordset->MoveNext();///移到下一条记录
				}
			}
			catch (...)//捕获所有异常
			{
				AfxMessageBox("请检查数据库中的数据是否正确！\n数据导入失败！");
				return;
			}
			m_pRecordset->Close();//关闭记录集

		}
#pragma endregion

		CString str;
		for (UINT i = 0; i < app->vec_P1.size(); i++)
		{
			m_pointsList.InsertItem(i, app->vec_P1[i].PNumb);//插入一行，并为该行所有元素赋初值
			str.Format(_T("%.3f"), app->vec_P1[i].x);
			m_pointsList.SetItemText(i, 1, str);//为单元格赋值
			str.Format(_T("%.3f"), app->vec_P1[i].y);
			m_pointsList.SetItemText(i, 2, str);
			str.Format(_T("%.3f"), app->vec_P1[i].z);
			m_pointsList.SetItemText(i, 3, str);
		}
	}
}

void CGRIDDlg::OnBnClickedBtnCaulate()//计算的事件处理程序
{
	// TODO: 在此添加控件通知处理程序代码

	CGRIDApp *app = (CGRIDApp *)AfxGetApp(); //生成指向应用程序类的指针
	app->vec_BBPoint.clear();

#pragma region 1.凸包生成
	//1.1 查找基点
	double min = app->vec_P1[0].y;
	int numb = 0;//存储最小值的点号信息
	for (UINT i = 0; i < app->vec_P1.size(); i++)
	{
		if (app->vec_P1[i].y < min)
		{
			min = app->vec_P1[i].y;
			numb = i;
		}
		else if ((app->vec_P1[i].y == min) && (app->vec_P1[i].x < app->vec_P1[numb].x))
		{
			numb = i;
		}
	}

	//1.2 计算夹角并排序
	CMyPoint p1;//基点与一号点互换位置,方便下一步循环
	p1 = app->vec_P1[0];
	app->vec_P1[0] = app->vec_P1[numb];
	app->vec_P1[numb] = p1;
	for (UINT i = 1; i < app->vec_P1.size(); i++)//计算每一个点与基点的夹角
	{
		double R = 0, x1, y1;
		x1 = app->vec_P1[i].x - app->vec_P1[0].x;
		y1 = app->vec_P1[i].y - app->vec_P1[0].y;
		if (x1 == 0)
		{
			app->vec_P1[i].angle = pi;
		}
		else
		{
			R = atan(abs(y1) / abs(x1));
			if (x1 < 0)
			{
				R = pi - R;
			}
			app->vec_P1[i].angle = R;
		}
	}
	//还需要加一个对于夹角相同时，剔除最近点的过程，但是不加也没关系，就是后面循环会多点，无所谓
	for (UINT i = 1; i < app->vec_P1.size(); i++)//夹角冒泡排序
	{
		for (UINT j = i + 1; j < app->vec_P1.size(); j++)
		{
			if (app->vec_P1[i].angle > app->vec_P1[j].angle)
			{
				p1 = app->vec_P1[i];
				app->vec_P1[i] = app->vec_P1[j];
				app->vec_P1[j] = p1;
			}
		}
	}

	//1.3 建立凸包点列表(难点)
	app->vec_BBPoint.push_back(app->vec_P1[0]);
	app->vec_BBPoint.push_back(app->vec_P1[1]);
	app->vec_BBPoint.push_back(app->vec_P1[2]);
	int top = 2;
	for (UINT i = 3; i < app->vec_P1.size(); i++)//建立凸包点列表
	{
		while ((app->vec_BBPoint[top - 1].x - app->vec_BBPoint[top].x) * (app->vec_P1[i].y - app->vec_BBPoint[top].y)
			- (app->vec_BBPoint[top - 1].y - app->vec_BBPoint[top].y) * (app->vec_P1[i].x - app->vec_BBPoint[top].x) > 0)//左转
		{
			top--;
		}
		if (top < app->vec_BBPoint.size() - 1)
		{
			app->vec_BBPoint[top + 1] = app->vec_P1[i];
		}
		else
		{
			app->vec_BBPoint.push_back(app->vec_P1[i]);
		}
		top++;
	}
	app->vec_BBPoint.push_back(app->vec_BBPoint[0]);

	/*CGRIDApp::report = "";
	for (UINT i = 0; i < app->vec_BBPoint.size(); i++)//计算每一个点与基点的夹角
	{
		CGRIDApp::report += app->vec_BBPoint[i].PNumb;
		CGRIDApp::report += "\n";
	}*/
#pragma endregion
#pragma region 2.格网生成
	// 2.1 建立最小外包矩形
	//找出最大及最小的x,y值
	max_x = app->vec_P1[0].x;
	max_y = app->vec_P1[0].y;
	min_x = app->vec_P1[0].x;
	min_y = app->vec_P1[0].y;
	for (UINT i = 0; i < app->vec_P1.size(); ++i)
	{
		if (app->vec_P1[i].x >= max_x)
		{
			max_x = app->vec_P1[i].x;
		}
		if (app->vec_P1[i].y >= max_y)
		{
			max_y = app->vec_P1[i].y;
		}
		if (app->vec_P1[i].x <= min_x)
		{
			min_x = app->vec_P1[i].x;
		}
		if (app->vec_P1[i].y <= min_y)
		{
			min_y = app->vec_P1[i].y;
		}
	}
	n_width = (int)((max_x - min_x) / bian) + 1;//存储格网点个数，比格网个数多一个
	n_height = (int)((max_y - min_y) / bian) + 1;

	/*CString str;
	str.Format("%d", n_width);
	MessageBox(str);*/

	//2.2判断网格中心是否在凸包内

	//创建二维数组存储格网信息
	CMyPoint **Grid_point;

	Grid_point = new CMyPoint*[n_width];//一个指针指向一个指针数组
	for (int j = 0; j < n_width; j++)
	{
		Grid_point[j] = new CMyPoint[n_height];//这个指针数组的每个指针元素又指向一个数组。
	}

	for (int i = 0; i < n_width; i++)//就算格网按格网点算，多一个，也不会对后面计算带来影响，因为不在凸包内
	{
		for (int j = 0; j < n_height; j++)//对每个凸包点都进行遍历
		{
			int nw = 0;
			CString str;
			str.Format("%d,%d",i,j);
			Grid_point[i][j].PNumb = str;
			Grid_point[i][j].x = ((i + i + 1) / 2) * bian + min_x;//计算格网中心点坐标
			Grid_point[i][j].y = ((j + j + 1) / 2) * bian + min_y;

			for (int c = 0; c < app->vec_BBPoint.size() - 1; c++)//按顺序遍历凸包点集,计算单边交点个数
			{
				double x1 = 0;
				if ((Grid_point[i][j].y >= app->vec_BBPoint[c].y && Grid_point[i][j].y <= app->vec_BBPoint[c + 1].y)
					|| (Grid_point[i][j].y <= app->vec_BBPoint[c].y && Grid_point[i][j].y >= app->vec_BBPoint[c + 1].y))//在两个y分量之间
				{
					x1 = (app->vec_BBPoint[c + 1].x - app->vec_BBPoint[c].x) * (Grid_point[i][j].y - app->vec_BBPoint[c].y)
						/ (app->vec_BBPoint[c + 1].y - app->vec_BBPoint[c].y) + app->vec_BBPoint[c].x;
					if (x1 > Grid_point[i][j].x)
					{
						nw++;
					}
				}
			}
			Grid_point[i][j].nw = nw;
		}
	}
	
#pragma endregion
#pragma region 3.体积计算

	//CString str = "";

	R0 = linyu * (n_width + n_height) / 2;//半径
	V = 0; V1 = 0; V2 = 0;
	for (int i = 0; i < n_width; i++)//计算体积
	{
		for (int j = 0; j < n_height; j++)//对每个格网点进行遍历
		{
			if (Grid_point[i][j].nw % 2 == 1)//单边交点个数为奇数时，则中心点C在凸包内
			{
				double h1, h2, h3, h4;
				//传入格网点坐标和半径以及散点数据，内插高程
				h1 = gaocheng(Grid_point[i][j].x - 0.5 * bian, Grid_point[i][j].y - 0.5 * bian, R0, app->vec_P1);
				h2 = gaocheng(Grid_point[i][j].x + 0.5 * bian, Grid_point[i][j].y - 0.5 * bian, R0, app->vec_P1);
				h3 = gaocheng(Grid_point[i][j].x - 0.5 * bian, Grid_point[i][j].y + 0.5 * bian, R0, app->vec_P1);
				h4 = gaocheng(Grid_point[i][j].x + 0.5 * bian, Grid_point[i][j].y + 0.5 * bian, R0, app->vec_P1);
				Grid_point[i][j].z = ((h1 + h2 + h3 + h4) / 4);
				Grid_point[i][j].TU = ((h1 + h2 + h3 + h4) / 4 - H0) * bian * bian;
				V += Grid_point[i][j].TU;
				if (Grid_point[i][j].TU > 0)
				{
					V1 += Grid_point[i][j].TU;
				}
				else
				{
					V2 += Grid_point[i][j].TU;
				}
				/*str += Grid_point[i][j].PNumb;
				CString str1;
				str1.Format("  %f  %f", Grid_point[i][j].x, Grid_point[i][j].y);
				str += str1;
				str1.Format("  %f", Grid_point[i][j].z);
				str += str1;
				str += "\n";*/
			}
		}
	}
	
	//app->report = str;

	//释放二维数组的内存
	for (int i = 0; i < n_width; i++)
	{
		delete[]Grid_point[i]; //先撤销指针元素所指向的数组
	}
	delete[] Grid_point;
#pragma endregion
#pragma region 生成计算报告
	//报告中基本信息
	CString str;
	app->report = "----------基本信息----------";
	app->report += "\r\n";
	app->report += "基准高程：9";
	app->report += "\r\n";
	app->report += "单位格网边长：2";
	app->report += "\r\n";
	app->report += "格网纵向个数：";
	str.Format("%d", n_height);
	app->report += str;
	app->report += "\r\n";
	app->report += "格网横向个数：";
	str.Format("%d", n_width);
	app->report += str;
	app->report += "\r\n";
	app->report += "格网总数：";
	str.Format("%d", n_height*n_width);
	app->report += str;
	app->report += "\r\n";
	app->report += "总体积：";
	str.Format(_T("%.4f"), V);
	app->report += str;
	app->report += "\r\n";
	/*app->report += "挖方：";
	str.Format(_T("%.4f"), V1);
	app->report += str;
	app->report += "\r\n";
	app->report += "填方：";
	str.Format(_T("%.4f"), V2);
	app->report += str;
	app->report += "\r\n";*/
	app->report += "\r\n";


	//报告中凸包点信息
	app->report += "----------凸包点信息----------";
	app->report += "\r\n";
	app->report += "点名    X坐标    Y坐标    H高程";
	app->report += "\r\n";
	for (int i = 0; i < app->vec_BBPoint.size(); ++i)
	{
		for (int j = 0; j < app->vec_P1.size(); ++j)
		{
			if (app->vec_P1[j].PNumb == app->vec_BBPoint[i].PNumb)
			{
				app->report += app->vec_P1[j].PNumb;
				app->report += "   ";
				str.Format(_T("%.3f"), app->vec_P1[j].x);
				app->report += str;
				app->report += "   ";
				str.Format(_T("%.3f"), app->vec_P1[j].y);
				app->report += str;
				app->report += "   ";
				str.Format(_T("%.3f"), app->vec_P1[j].z);
				app->report += str;
			}
		}
		app->report += "\r\n";
	}
	
#pragma endregion
}

void CGRIDDlg::OnBnClickedBtnMap()//绘图的事件处理程序
{
	// TODO: 在此添加控件通知处理程序代码
	CMapDlg dlg;
	dlg.DoModal();
}


void CGRIDDlg::OnBnClickedBtnResult()//生成计算报告的事件处理程序
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(CGRIDApp::report, _T("计算报告"));//直接使用静态变量
}


void CGRIDDlg::OnBnClickedBtnSave()//保存计算报告的事件处理程序
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fileDlg(FALSE);
	fileDlg.m_ofn.lpstrTitle = "文件保存";
	fileDlg.m_ofn.lpstrFilter = "Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";
	fileDlg.m_ofn.lpstrDefExt = "txt";
	if (IDOK == fileDlg.DoModal())
	{
		CFile file(fileDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite);
		file.Write(CGRIDApp::report, CGRIDApp::report.GetLength());
		file.Close();
		MessageBox("文件保存成功！");
	}
}

double CGRIDDlg::gaocheng(double x, double y, double r, vector<CMyPoint> M)//反距离加权法计算高程
{
	// TODO: 在此处添加实现代码.
	double d = 0, H;
	double sum_hi = 0, sum_di = 0;
	vector<double> hi;
	vector<double> di;
	for (UINT i = 0; i < M.size(); i++)
	{
		d = sqrt((x - M[i].x) * (x - M[i].x) + (y - M[i].y) * (y - M[i].y));
		if (d <= r)
		{
			hi.push_back(M[i].z / d);
			di.push_back(1 / d);
		}
	}
	for (UINT i = 0; i < hi.size(); i++)
	{
		sum_hi += hi[i];
	}
	for (UINT i = 0; i < di.size(); i++)
	{
		sum_di += di[i];
	}
	H = sum_hi / sum_di;
	return H;
}
