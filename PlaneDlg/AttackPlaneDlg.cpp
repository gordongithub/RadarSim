
// PlaneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AttackPlane.h"
#include "AttackPlaneDlg.h"

#include "Utility.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAttackPlaneDlg �Ի���




CAttackPlaneDlg::CAttackPlaneDlg(LPCWSTR title, CWnd* pParent /*=NULL*/)
	: CPlaneDlg(title, pParent)
    , m_NaviAlgo(NULL)
{
    m_DlgType = DlgTypeAttackPlane;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // m_MatlabDlg = new CMatlabDlg("attack_matlab_dialog", "attack_plane_true", "attack_target_true", "attack_target_fusion", "attack_target_filter", "attack_global_var");
    // m_ShowDataListDlg = true;
    m_AddMissile = true;
}

CAttackPlaneDlg::~CAttackPlaneDlg()
{
    // delete m_MatlabDlg;
    // m_MatlabDlg = NULL;
}

BEGIN_MESSAGE_MAP(CAttackPlaneDlg, CPlaneDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CAttackPlaneDlg ��Ϣ�������

BOOL CAttackPlaneDlg::OnInitDialog()
{
	CPlaneDlg::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAttackPlaneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAttackPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAttackPlaneDlg::ResetCtrls()
{
    CPlaneDlg::ResetCtrls();
    m_NaviOutput = NaviOutput();
    m_Missiles.clear();
}

void CAttackPlaneDlg::ConnectDataCenter()
{
    CPlaneDlg::ConnectDataCenter();
    m_DataCenterSocket->SendAttack();
}

void CAttackPlaneDlg::SendNoiseData(NoiseDataPacket &packet)
{
}

void CAttackPlaneDlg::SetNaviAlgo(NaviAlgo *algo)
{
    if (m_NaviAlgo)
    {
        delete m_NaviAlgo;
        m_NaviAlgo = NULL;
    }
    m_NaviAlgo = algo;
    if (!m_NaviAlgo->Init())
    {
        CString msg;
        msg.AppendFormat(TEXT("�����㷨%s��ʼ��ʧ��."), m_NaviAlgo->m_Name);
    }
    GetDlgItem(IDC_NAVI_ALGO)->SetWindowText(m_NaviAlgo->m_Name);
}

void CAttackPlaneDlg::AddTrueData(TrueDataPacket &packet)
{
    // CPlaneDlg::AddTrueData(packet);
    m_Plane.MoveTo(packet.m_PlaneTrueData.m_Pos);

    // ��ʾ��֡ǰ�벿��������������
    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddPlaneTrueData(0, packet.m_PlaneTrueData.m_Pos);
    }
    m_StateMap.AddPlaneData(0, packet.m_PlaneTrueData.m_Pos, packet.m_PlaneTrueData.m_Vel, (TargetState)packet.m_PlaneTrueData.m_State);

    // �ù������Ĵ������ճ�����������������ʾ
    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        if (packet.m_TargetTrueDatas[i].m_State != TargetStateAlive)
        {
            continue;
        }
        Position rel = packet.m_TargetTrueDatas[i].m_Pos - packet.m_PlaneTrueData.m_Pos;

        m_Radar.AddTargetData(i, rel);
        m_Esm.AddTargetData(i, rel);
        m_Infrared.AddTargetData(i, rel);

        m_DataList.AddTargetData(i, packet.m_TargetTrueDatas[i].m_Time);

        if (m_MatlabDlg)
        {
            m_MatlabDlg->AddTargetTrueData(i, packet.m_TargetTrueDatas[i].m_Pos);
        }
    }

    m_RadarCtrl.DrawTargets();
    m_RadarCtrl.BlendAll();
    m_RadarCtrl.Invalidate();

    m_RadarDlg.m_Ctrl->DrawTargets();
    m_RadarDlg.m_Ctrl->BlendAll();
    m_RadarDlg.m_Ctrl->Invalidate();

    m_EsmCtrl.DrawTargets();
    m_EsmCtrl.BlendAll();
    m_EsmCtrl.Invalidate();

    m_EsmDlg.m_Ctrl->DrawTargets();
    m_EsmDlg.m_Ctrl->BlendAll();
    m_EsmDlg.m_Ctrl->Invalidate();

    m_InfraredCtrl.DrawTargets();
    m_InfraredCtrl.BlendAll();
    m_InfraredCtrl.Invalidate();

    m_InfraredDlg.m_Ctrl->DrawTargets();
    m_InfraredDlg.m_Ctrl->BlendAll();
    m_InfraredDlg.m_Ctrl->Invalidate();

    m_DataListCtrl.AddTargetData();
    m_DataListDlg.m_Ctrl->AddTargetData();

    // ���д˴����Կ���Ŀ����ֵ�������������жϱ�ը
    Utility::CheckMissileHit(m_Missiles, packet.m_TargetTrueDatas);
    m_TargetStates.clear();
    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        m_TargetStates.push_back((TargetState)packet.m_TargetTrueDatas[i].m_State);
    }
}

void CAttackPlaneDlg::AddOtherTrueData(int i, TrueDataFrame &frame)
{
    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddPlaneTrueData(i + 1, frame.m_Pos);
    }

    m_StateMap.AddPlaneData(i + 1, frame.m_Pos, frame.m_Vel, (TargetState)frame.m_State);
    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();
}

void CAttackPlaneDlg::AddControlData(ControlDataPacket &packet)
{
    // ��ʾ��֡��벿����̬�Ʋ��֣�Ŀ��͵���
    for (int i = 0; i < packet.m_FusionData.m_FusionDatas.size(); ++i)
    {
        TrueDataFrame &frame = packet.m_FusionData.m_FusionDatas[i];
        m_StateMap.AddTargetData(i, frame.m_Pos, frame.m_Vel, m_TargetStates[i]);
    }

    for (int i = 0; i < m_Missiles.size(); ++i)
    {
        m_StateMap.AddMissileData(i, m_Missiles[i].m_Position, m_Missiles[i].m_Vel, m_Missiles[i].m_State);
    }

    m_StateMapDlg.m_Ctrl.DrawTargets();
    m_StateMapDlg.m_Ctrl.BlendAll();
    m_StateMapDlg.m_Ctrl.Invalidate();

    // У����֡������ɵ������ƹ������͵�������һ֡λ��

    DoNavi(packet);

    ControlDataAckPacket accPacket;
    accPacket.m_PlaneTrueData.m_Time = packet.m_FusionData.m_FusionDatas[0].m_Time;
    accPacket.m_PlaneTrueData.m_Id = m_Plane.m_Id;
    accPacket.m_PlaneTrueData.m_Pos = m_Plane.m_Position;
    accPacket.m_PlaneTrueData.m_Vel = m_Plane.m_Vel;
    accPacket.m_PlaneTrueData.m_Acc = m_Plane.m_Acc;
    accPacket.m_PlaneTrueData.m_State = m_Plane.m_State;

    for (int i = 0; i < m_Missiles.size(); ++i)
    {
        TrueDataFrame frame;
        frame.m_Time = packet.m_FusionData.m_FusionDatas[0].m_Time;
        frame.m_Id = m_Missiles[i].m_Id;
        frame.m_Pos = m_Missiles[i].m_Position;
        frame.m_Vel = m_Missiles[i].m_Vel;
        frame.m_Acc = m_Missiles[i].m_Acc;
        // ����¼����״̬������DataCenter�Լ�������Ŀ������λ�����ж�
        // frame.m_State = m_Missiles[i].m_State;
        accPacket.m_MissileTrueDatas.push_back(frame);
    }
    m_FusionSocket->SendControlDataAck(accPacket);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
void CAttackPlaneDlg::DoNavi(const ControlDataPacket &packet)
{
    NaviInput input;
    input.m_FusionDatas = packet.m_FusionData.m_FusionDatas;
    input.m_FilterDatas = packet.m_FusionData.m_FilterDatas;
    input.m_ControlData = packet.m_ControlData;
    input.m_Interval = m_GlobalData.m_Interval;
    input.m_PlaneTrueData.m_Time = packet.m_FusionData.m_NoiseDatas[0].m_PlaneTrueData.m_Time;
    input.m_PlaneTrueData.m_Id = m_Plane.m_Id;
    input.m_PlaneTrueData.m_Pos = m_Plane.m_Position;
    input.m_PlaneTrueData.m_Vel = m_Plane.m_Vel;
    input.m_PlaneTrueData.m_Acc = m_Plane.m_Acc;
    for (int i = 0; i < m_Missiles.size(); ++i)
    {
        TrueDataFrame missileTrueData;
        missileTrueData.m_Time = packet.m_FusionData.m_NoiseDatas[0].m_PlaneTrueData.m_Time;
        missileTrueData.m_Id = m_Missiles[i].m_Id;
        missileTrueData.m_Pos = m_Missiles[i].m_Position;
        missileTrueData.m_Vel = m_Missiles[i].m_Vel;
        missileTrueData.m_Acc = m_Missiles[i].m_Acc;
        input.m_MissileTrueDatas.push_back(missileTrueData);
    }
    m_NaviOutput = NaviOutput();
    for (int i = 0; i < m_Missiles.size(); ++i)
    {
        m_NaviOutput.m_MissileTrueDatas.push_back(TrueDataFrame());
    }

    if (!m_NaviAlgo)
    {
        AfxMessageBox(TEXT("��δָ�������㷨."));
        return;
    }
    if (!m_NaviAlgo->Run(input, m_NaviOutput))
    {
        AfxMessageBox(TEXT("�����㷨���д���."));
        return;
    }
    m_Plane.MoveTo(m_NaviOutput.m_PlaneTrueData.m_Pos);
    m_Plane.m_Vel = m_NaviOutput.m_PlaneTrueData.m_Vel;
    m_Plane.m_Acc = m_NaviOutput.m_PlaneTrueData.m_Acc;

    for (int i = 0; i < m_NaviOutput.m_MissileTrueDatas.size(); ++i)
    {
        if (m_NaviOutput.m_MissileTrueDatas[i].m_State == TargetStateAlive)
        {
            m_Missiles[i].MoveTo(m_NaviOutput.m_MissileTrueDatas[i].m_Pos);
            m_Missiles[i].m_Vel = m_NaviOutput.m_MissileTrueDatas[i].m_Vel;
            m_Missiles[i].m_Acc = m_NaviOutput.m_MissileTrueDatas[i].m_Acc;
        }
        else if (m_NaviOutput.m_MissileTrueDatas[i].m_State == TargetStateExploding)
        {
            m_Missiles[i].m_Position = Position(0, 0, 0);
            m_Missiles[i].m_Vel = Point3D(0, 0, 0);
            m_Missiles[i].m_Acc = Point3D(0, 0, 0);
        }
    }
}
