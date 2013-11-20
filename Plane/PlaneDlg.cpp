
// PlaneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Plane.h"
#include "PlaneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneDlg 对话框




CPlaneDlg::CPlaneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlaneDlg::IDD, pParent)
    , m_Initialized(false)
    , m_ShowRadarDlg(true)
    , m_Radar(Sensor::SensorTypeSource, m_Plane)
    , m_RadarCtrl(m_Radar)
    , m_PlaneRadarProxy(*this)
    , m_RadarDlg(m_Radar, m_PlaneRadarProxy)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlaneDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RADAR_CTRL, m_RadarCtrl);
}

BEGIN_MESSAGE_MAP(CPlaneDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_SIZE()
    ON_STN_DBLCLK(IDC_RADAR_CTRL, &CPlaneDlg::OnStnDblclickRadarCtrl)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CPlaneDlg 消息处理程序

BOOL CPlaneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码
    m_Initialized = true;

    // 初始化我机和目标
    Target target0, target1;

    target0.m_Id = 3;
    target1.m_Id = 4;

    target0.m_Color = Target::TargetColorOrange;
    target1.m_Color = Target::TargetColorYellow;

    m_Plane.AddTarget(target0);
    m_Plane.AddTarget(target1);

    // 初始化雷达
    if (!m_RadarCtrl.Init())
    {
        return FALSE;
    }
    Resize();

    CRadarDlg::CreateDlg(m_RadarDlg);
    if (m_ShowRadarDlg)
    {
        m_RadarDlg.ShowWindow(SW_SHOW);
    }
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // Debug用的Timer
    SetTimer(0, 800, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPlaneDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlaneDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    // Resize Radar.
    int left = rect.left + PAD, width = (rect.right - rect.left) / 2 - PAD * 2, top = rect.top + PAD, height = (rect.bottom - rect.top) / 2 - PAD * 2;
    if (width > height)
    {
        width = height;
    }
    GetDlgItem(IDC_RADAR_CTRL_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = width - PAD *2;
    top = top + PAD * 2;
    height = height - PAD * 3;
    m_RadarCtrl.MoveWindow(left, top, width, height);
}

void CPlaneDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (m_Initialized)
    {
        Resize();
    }
}

void CPlaneDlg::OnStnDblclickRadarCtrl()
{
    // TODO: 在此添加控件通知处理程序代码
    if (m_ShowRadarDlg)
    {
        m_RadarDlg.ShowWindow(SW_HIDE);
        m_ShowRadarDlg = false;
    }
    else
    {
        m_RadarDlg.ShowWindow(SW_SHOW);
        m_ShowRadarDlg = true;
    }
}

void CPlaneDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialog::OnTimer(nIDEvent);

    m_Plane.m_Targets[3].MoveTo(m_Plane.m_Targets[3].m_Position + Position(2, 3, 4));
    m_Plane.m_Targets[4].MoveTo(m_Plane.m_Targets[4].m_Position + Position(3, 5, 4));

    m_Plane.MoveTo(m_Plane.m_Position + Position(1, 1, 1));

    m_RadarCtrl.DrawTargets();
    m_RadarCtrl.BlendAll();
    m_RadarCtrl.Invalidate();

    m_RadarDlg.m_Ctrl.DrawTargets();
    m_RadarDlg.m_Ctrl.BlendAll();
    m_RadarDlg.m_Ctrl.Invalidate();
}
