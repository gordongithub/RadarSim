// SensorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SensorDlg.h"


// CSensorDlg �Ի���

IMPLEMENT_DYNAMIC(CSensorDlg, CDialog)

CSensorDlg::CSensorDlg(LPCWSTR title, Sensor &sensor, CCommonDlg *dlg, CWnd* pParent /*=NULL*/)
	: CDialog(CSensorDlg::IDD, pParent)
    , m_Title(title)
    , m_Sensor(sensor)
    , m_Dlg(dlg)
    , m_Initialized(false)
    , m_SensorMaxDis(TEXT("0"))
    , m_SensorMaxTheta(TEXT("0"))
    , m_SensorMaxPhi(TEXT("0"))
    , m_SensorDisVar(TEXT("0"))
    , m_SensorThetaVar(TEXT("0"))
    , m_SensorPhiVar(TEXT("0"))
    , m_SensorProDet(TEXT("0"))
{
    m_Ctrl = new CSensorCtrl(m_Sensor);
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSensorDlg::~CSensorDlg()
{
    delete m_Ctrl;
}

void CSensorDlg::CreateDlg(CSensorDlg &dlg)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    dlg.Create(IDD_SENSOR_DLG, GetDesktopWindow());
}

void CSensorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SENSOR, *m_Ctrl);
    DDX_Text(pDX, IDC_SENSOR_ID, SensorIdNames[m_Sensor.m_Id]);
    DDX_Check(pDX, IDC_SENSOR_ENABLE, m_Sensor.m_Enable);
    DDX_Text(pDX, IDC_SENSOR_MAX_DIS, m_SensorMaxDis);
    DDX_Text(pDX, IDC_SENSOR_MAX_THETA, m_SensorMaxTheta);
    DDX_Text(pDX, IDC_SENSOR_MAX_PHI, m_SensorMaxPhi);
    DDX_Text(pDX, IDC_SENSOR_DIS_VAR, m_SensorDisVar);
    DDX_Text(pDX, IDC_SENSOR_THETA_VAR, m_SensorThetaVar);
    DDX_Text(pDX, IDC_SENSOR_PHI_VAR, m_SensorPhiVar);
    DDX_Text(pDX, IDC_SENSOR_PRO_DET, m_SensorProDet);
    DDX_Check(pDX, IDC_SENSOR_SHOW_SCANLINE, m_Sensor.m_ShowScanline);
    DDX_Check(pDX, IDC_SENSOR_SHOW_TRACK, m_Sensor.m_ShowTrack);
    DDX_Control(pDX, IDC_SENSOR_TARGET_ID, m_TargetId);
    DDX_Control(pDX, IDC_SENSOR_TARGET_COLOR, m_TargetColor);
}


BEGIN_MESSAGE_MAP(CSensorDlg, CDialog)
	ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_SENSOR_SHOW_SCANLINE, &CSensorDlg::OnBnClickedSensorShowScanline)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_SENSOR_ENABLE, &CSensorDlg::OnBnClickedSensorEnable)
    ON_BN_CLICKED(IDC_SENSOR_SHOW_TRACK, &CSensorDlg::OnBnClickedSensorShowTrack)
    ON_EN_CHANGE(IDC_SENSOR_MAX_DIS, &CSensorDlg::OnEnChangeSensorMaxDis)
    ON_CBN_SELCHANGE(IDC_SENSOR_TARGET_ID, &CSensorDlg::OnCbnSelchangeSensorTargetId)
    ON_CBN_SELCHANGE(IDC_SENSOR_TARGET_COLOR, &CSensorDlg::OnCbnSelchangeSensorTargetColor)
    ON_EN_CHANGE(IDC_SENSOR_MAX_THETA, &CSensorDlg::OnEnChangeSensorMaxTheta)
    ON_EN_CHANGE(IDC_SENSOR_MAX_PHI, &CSensorDlg::OnEnChangeSensorMaxPhi)
    ON_EN_CHANGE(IDC_SENSOR_DIS_VAR, &CSensorDlg::OnEnChangeSensorDisVar)
    ON_EN_CHANGE(IDC_SENSOR_THETA_VAR, &CSensorDlg::OnEnChangeSensorThetaVar)
    ON_EN_CHANGE(IDC_SENSOR_PHI_VAR, &CSensorDlg::OnEnChangeSensorPhiVar)
    ON_EN_CHANGE(IDC_SENSOR_PRO_DET, &CSensorDlg::OnEnChangeSensorProDet)
END_MESSAGE_MAP()


// CSensorDlg ��Ϣ�������

BOOL CSensorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_Initialized = true;

    SetWindowTextW(m_Title);
    GetDlgItem(IDC_SENSOR_GRP)->SetWindowTextW(m_Title);

    for (int i = 0; i < TargetColorLast; ++i)
    {
        m_TargetColor.InsertString(i, TargetColorNames[i]);
    }

    Resize();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSensorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSensorDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    // CDialog::OnClose();
    m_Dlg->OnSubDlgClose(this);
}

void CSensorDlg::Resize()
{
    RECT rect;
    GetClientRect(&rect);

    int left = rect.left + PAD, width = 2 * (80 + PAD), top = rect.top + PAD, height = rect.bottom - rect.top - PAD * 2;
    int left2 = left + width + PAD, width2 = rect.right - PAD - left2, top2 = top, height2 = height;

    GetDlgItem(IDC_SENSOR_PARAM_GRP)->MoveWindow(left, top, width, height);

    left = left + PAD;
    width = 80;
    top = top + PAD * 2;
    height = 20;
    GetDlgItem(IDC_SENSOR_ID_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_ID)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_ENABLE)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_DIS_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_DIS)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_THETA_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_THETA)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_MAX_PHI_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_MAX_PHI)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_DIS_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_DIS_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_THETA_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_THETA_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_PHI_VAR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_PHI_VAR)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_PRO_DET_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_PRO_DET)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_SHOW_SCANLINE)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_SHOW_TRACK)->MoveWindow(left, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_TARGET_ID_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TARGET_ID)->MoveWindow(left + width, top, width, height);

    top = top + height + PAD;
    GetDlgItem(IDC_SENSOR_TARGET_COLOR_STATIC)->MoveWindow(left, top, width, height);
    GetDlgItem(IDC_SENSOR_TARGET_COLOR)->MoveWindow(left + width, top, width, height);

    GetDlgItem(IDC_SENSOR_GRP)->MoveWindow(left2, top2, width2, height2);
    GetDlgItem(IDC_SENSOR)->MoveWindow(left2 + PAD, top2 + PAD * 2, width2 - PAD * 2, height2 - PAD * 3);
}

void CSensorDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: �ڴ˴������Ϣ����������
    if (m_Initialized)
    {
        Resize();
    }
}

void CSensorDlg::OnBnClickedSensorEnable()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_Sensor.m_Enable = !m_Sensor.m_Enable;
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgEnable(this);
}

void CSensorDlg::OnBnClickedSensorShowScanline()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_Sensor.m_ShowScanline = !m_Sensor.m_ShowScanline;
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgShowScanline(this);
}

void CSensorDlg::OnBnClickedSensorShowTrack()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    m_Sensor.m_ShowTrack = !m_Sensor.m_ShowTrack;
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgShowTrack(this);
}

void CSensorDlg::OnCbnSelchangeSensorTargetId()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_TargetId.GetCurSel();
    int count = m_TargetId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_TargetColor.SetCurSel(m_Sensor.m_TargetColors[index]);
    }
}

void CSensorDlg::OnCbnSelchangeSensorTargetColor()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int index = m_TargetId.GetCurSel();
    int count = m_TargetId.GetCount();
    if ((index != CB_ERR) && (count >= 1))
    {
        m_Sensor.m_TargetColors[index] = (TargetColor)m_TargetColor.GetCurSel();
        m_Ctrl->DrawTargets();
        m_Ctrl->BlendAll();
        m_Ctrl->Invalidate();

        m_Dlg->OnSubDlgTargetColor(this);
    }
}

void CSensorDlg::Reset()
{
    m_Ctrl->Reset();

    m_SensorMaxDis.Format(TEXT("%lf"), m_Sensor.m_MaxDis);
    m_SensorMaxTheta.Format(TEXT("%lf"), m_Sensor.m_MaxTheta);
    m_SensorMaxPhi.Format(TEXT("%lf"), m_Sensor.m_MaxPhi);
    m_SensorDisVar.Format(TEXT("%lf"), m_Sensor.m_DisVar);
    m_SensorThetaVar.Format(TEXT("%lf"), m_Sensor.m_ThetaVar);
    m_SensorPhiVar.Format(TEXT("%lf"), m_Sensor.m_PhiVar);
    m_SensorProDet.Format(TEXT("%lf"), m_Sensor.m_ProDet);

    m_TargetId.ResetContent();
    m_TargetColor.SetCurSel(CB_ERR);

    UpdateData(FALSE);
}

void CSensorDlg::AddTarget(Target &target)
{
    CString str;
    str.AppendFormat(TEXT("%d"), target.m_Id);
    m_TargetId.InsertString(m_TargetId.GetCount(), str);
}

void CSensorDlg::OnEnChangeSensorMaxDis()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_SENSOR_MAX_DIS, m_SensorMaxDis);
    m_Sensor.m_MaxDis = _tcstod((LPCWSTR)m_SensorMaxDis, 0);
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgMaxDis(this);
}

void CSensorDlg::OnEnChangeSensorMaxTheta()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_SENSOR_MAX_THETA, m_SensorMaxTheta);
    m_Sensor.m_MaxTheta = _tcstod((LPCWSTR)m_SensorMaxTheta, 0);
    m_Ctrl->DrawThetaRange();
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgMaxTheta(this);
}

void CSensorDlg::OnEnChangeSensorMaxPhi()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_SENSOR_MAX_PHI, m_SensorMaxPhi);
    m_Sensor.m_MaxPhi = _tcstod((LPCWSTR)m_SensorMaxPhi, 0);
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgMaxPhi(this);
}

void CSensorDlg::OnEnChangeSensorDisVar()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_SENSOR_DIS_VAR, m_SensorDisVar);
    m_Sensor.m_DisVar = _tcstod((LPCWSTR)m_SensorDisVar, 0);
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgDisVar(this);
}

void CSensorDlg::OnEnChangeSensorThetaVar()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_SENSOR_THETA_VAR, m_SensorThetaVar);
    m_Sensor.m_ThetaVar = _tcstod((LPCWSTR)m_SensorThetaVar, 0);
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgThetaVar(this);
}

void CSensorDlg::OnEnChangeSensorPhiVar()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_SENSOR_PHI_VAR, m_SensorPhiVar);
    m_Sensor.m_PhiVar = _tcstod((LPCWSTR)m_SensorPhiVar, 0);
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgPhiVar(this);
}

void CSensorDlg::OnEnChangeSensorProDet()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    GetDlgItemTextW(IDC_SENSOR_PRO_DET, m_SensorProDet);
    m_Sensor.m_ProDet = _tcstod((LPCWSTR)m_SensorProDet, 0);
    m_Ctrl->DrawTargets();
    m_Ctrl->BlendAll();
    m_Ctrl->Invalidate();

    m_Dlg->OnSubDlgProDet(this);
}
