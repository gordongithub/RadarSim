// ZoomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZoomDlg.h"


// CZoomDlg �Ի���

IMPLEMENT_DYNAMIC(CZoomDlg, CDialog)

CZoomDlg::CZoomDlg(CCommonDlg *dlg, CWnd* pParent /*=NULL*/)
	: CDialog(CZoomDlg::IDD, pParent)
    , m_Dlg(dlg)
    , m_Image(NULL)
    , m_Initialized(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CZoomDlg::~CZoomDlg()
{
}

void CZoomDlg::CreateDlg(CZoomDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_ZOOM_DLG, GetDesktopWindow());
}

void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CZoomDlg::DrawTarget()
{
    Image *keyTarget = Image::FromFile(KEY_TARGET_FILE_NAME);
    if (m_Image)
    {
        delete m_Image;
    }
    m_Image = keyTarget;
}

BEGIN_MESSAGE_MAP(CZoomDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CZoomDlg ��Ϣ�������

void CZoomDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: �ڴ˴������Ϣ����������
    // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
    if (m_Image)
    {
        RECT rect;
        GetWindowRect(&rect);
        ScreenToClient(&rect);

        double left = rect.left + PAD,
            top = rect.top + PAD;
        Gdiplus::Graphics graphics(dc.GetSafeHdc());
        graphics.DrawImage(m_Image, PointF(left, top));
    }
}

BOOL CZoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_Initialized = true;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CZoomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZoomDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    // CDialog::OnClose();
    m_Dlg->OnSubDlgClose(this);
}
