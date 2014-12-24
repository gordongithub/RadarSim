// PlaneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FusionPlane.h"
#include "FusionPlaneDlg.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFusionPlaneDlg �Ի���




CFusionPlaneDlg::CFusionPlaneDlg(LPCWSTR title, CWnd* pParent /*=NULL*/)
	: CPlaneDlg(title, pParent)
    , m_FusionSocket(0)
    , m_FusionAlgo(NULL)
{
    m_DlgType = DlgTypeFusionPlane;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_FusionSocket = new FusionSocket(this);
    m_AddMissile = true;

    m_MatlabDlg = new CMatlabDlg3("attack_matlab_dialog.dll", "attack_matlab_dialog");
    // m_ShowDataListDlg = true;
}

CFusionPlaneDlg::~CFusionPlaneDlg()
{
    delete m_MatlabDlg;
    m_MatlabDlg = NULL;
}

BEGIN_MESSAGE_MAP(CFusionPlaneDlg, CPlaneDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CFusionPlaneDlg ��Ϣ�������

BOOL CFusionPlaneDlg::OnInitDialog()
{
    if (!m_FusionSocket->Create())
    {
        AfxMessageBox(TEXT("�׽��ִ���ʧ��"));
        exit(-1);
    }
    if (!m_FusionSocket->Listen())
    {
        AfxMessageBox(TEXT("����ʧ��"));
        exit(-1);
    }
    if (!m_FusionSocket->AsyncSelect(FD_ACCEPT))
    {
        AfxMessageBox(TEXT("ѡ��ʧ��"));
        exit(-1);
    }

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

void CFusionPlaneDlg::OnPaint()
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
HCURSOR CFusionPlaneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFusionPlaneDlg::ConnectDataCenter()
{
    CPlaneDlg::ConnectDataCenter();
    CString addr;
    UINT port;
    m_FusionSocket->GetSockName(addr, port);
    m_DataCenterSocket->SendFusionAddr(port);
}

void CFusionPlaneDlg::ConnectFusion(const CString &addr, int port)
{
}

void CFusionPlaneDlg::AddPlaneSocket()
{
    m_Lock.Lock();
    PlaneSocket *socket = new PlaneSocket(this);
    if (m_FusionSocket->Accept(*socket))
    {
        socket->AsyncSelect(FD_CLOSE | FD_READ | FD_WRITE);
    }
    m_PlaneSockets.push_back(socket);
    m_Lock.Unlock();
}

void CFusionPlaneDlg::AddTrueData(TrueDataPacket &packet)
{
    // CPlaneDlg::AddTrueData(packet);
    m_Plane.MoveTo(packet.m_PlaneTrueData.m_Pos);

    // ��ʾ��֡ǰ�벿��������������
    if (m_MatlabDlg)
    {
        m_MatlabDlg->AddPlaneTrueData(0, packet.m_PlaneTrueData.m_Pos);
    }
    m_StateMap.AddPlaneData(0, packet.m_PlaneTrueData.m_Pos, packet.m_PlaneTrueData.m_Vel, (TargetState)packet.m_PlaneTrueData.m_State);

    // ���ںϻ��Ĵ������ճ�����������������ʾ
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

    m_TargetStates.clear();
    for (int i = 0; i < packet.m_TargetTrueDatas.size(); ++i)
    {
        m_TargetStates.push_back((TargetState)packet.m_TargetTrueDatas[i].m_State);
    }
}

void CFusionPlaneDlg::AddNoiseData(SocketPacketPair spp)
{
    m_NoiseDatas.insert(make_pair(spp.second.m_PlaneTrueData.m_Id, spp));
    if (m_NoiseDatas.size() == m_PlaneSockets.size())
    {
        DoFusion();

        for (int i = 0; i < m_FusionOutput.m_FusionData.m_FusionDatas.size(); ++i)
        {
            TrueDataFrame &fusionFrame = m_FusionOutput.m_FusionData.m_FusionDatas[i];
            m_MatlabDlg->AddTargetFusionData(i, fusionFrame);
            TrueDataFrame &filterFrame = m_FusionOutput.m_FusionData.m_FilterDatas[i];
            m_MatlabDlg->AddTargetFilterData(i, filterFrame);
            // m_MatlabDlg->UpdateGlobalVar();
        }

        m_MatlabDlg->Update();

        // ��ʾ��֡��벿����̬�Ʋ��֣�Ŀ��͵���
        for (int i = 0; i < m_FusionOutput.m_FusionData.m_FusionDatas.size(); ++i)
        {
            TrueDataFrame &frame = m_FusionOutput.m_FusionData.m_FusionDatas[i];
            m_StateMap.AddTargetData(i, frame.m_Pos, frame.m_Vel, m_TargetStates[i]);
        }

        m_StateMapDlg.m_Ctrl.DrawTargets();
        m_StateMapDlg.m_Ctrl.BlendAll();
        m_StateMapDlg.m_Ctrl.Invalidate();

        for (int i = 0; i < m_PlaneSockets.size(); ++i)
        {
            m_PlaneSockets[i]->SendControlData(m_FusionOutput.m_ControlDatas[i]);
        }

        m_NoiseDatas.clear();
    }
}

void CFusionPlaneDlg::SendNoiseData(NoiseDataPacket &packet)
{
    AddNoiseData(make_pair((PlaneSocket *)NULL, packet));
}

void CFusionPlaneDlg::SetFusionAlgo(FusionAlgo *algo)
{
    if (m_FusionAlgo)
    {
        delete m_FusionAlgo;
        m_FusionAlgo = NULL;
    }
    m_FusionAlgo = algo;
    if (!m_FusionAlgo->Init())
    {
        CString msg;
        msg.AppendFormat(TEXT("�ں��㷨%s��ʼ��ʧ��."), m_FusionAlgo->m_Name);
    }
    GetDlgItem(IDC_FUSION_ALGO)->SetWindowText(m_FusionAlgo->m_Name);
}

void CFusionPlaneDlg::DoFusion()
{
    if (!m_FusionAlgo)
    {
        AfxMessageBox(TEXT("��δָ���ں��㷨."));
        return;
    }
    FusionInput input;
    for (map<int, SocketPacketPair>::iterator it = m_NoiseDatas.begin(); it != m_NoiseDatas.end(); ++it)
    {
        input.m_NoiseDataPackets.push_back(it->second.second);
    }
    input.m_Interval = m_GlobalData.m_Interval;
    input.m_InfraredMaxDis = m_Infrared.m_MaxDis;
    m_FusionOutput = FusionOutput();
    if (!m_FusionAlgo->Run(input, m_FusionOutput))
    {
        AfxMessageBox(TEXT("�ں��㷨���д���."));
        return;
    }

    // Set Plane true data.
    for (map<int, SocketPacketPair>::iterator it = m_NoiseDatas.begin(); it != m_NoiseDatas.end(); ++it)
    {
        m_FusionOutput.m_FusionData.m_PlaneTrueDatas.push_back(it->second.second.m_PlaneTrueData);
    }

    // Check fusion output.
    int planeSize = m_NoiseDatas.size();
    int targetSize = m_NoiseDatas.begin()->second.second.m_TargetNoiseDatas.size();
    if (m_FusionOutput.m_FusionData.m_FusionDatas.size() != targetSize)
    {
        CString msg;
        msg.AppendFormat(TEXT("�ں��㷨������ں����ݸ���(%d)������ʵ�ʵл�����(%d)����������ں��㷨��"), m_FusionOutput.m_FusionData.m_FusionDatas.size(), targetSize);
        AfxMessageBox(msg);
        return;
    }
    if (m_FusionOutput.m_FusionData.m_FilterDatas.size() != targetSize)
    {
        CString msg;
        msg.AppendFormat(TEXT("�ں��㷨������˲����ݸ���(%d)������ʵ�ʵл�����(%d)����������ں��㷨��"), m_FusionOutput.m_FusionData.m_FilterDatas.size(), targetSize);
        AfxMessageBox(msg);
        return;
    }
    if (m_FusionOutput.m_FusionData.m_NoiseDatas.size() != planeSize)
    {
        CString msg;
        msg.AppendFormat(TEXT("�ں��㷨������������ݸ���(%d)������ʵ���һ�����(%d)����������ں��㷨��"), m_FusionOutput.m_FusionData.m_NoiseDatas.size(), planeSize);
        AfxMessageBox(msg);
        return;
    }
    if (m_FusionOutput.m_ControlDatas.size() != planeSize)
    {
        CString msg;
        msg.AppendFormat(TEXT("�ں��㷨����Ŀ������ݸ���(%d)������ʵ���һ�����(%d)����������ں��㷨��"), m_FusionOutput.m_ControlDatas.size(), planeSize);
        AfxMessageBox(msg);
        return;
    }

#ifdef _DEV
    for (int i = 0; i < m_FusionOutput.m_FusionData.m_FusionDatas.size(); ++i)
    {
        TrueDataFrame &frame = m_FusionOutput.m_FusionData.m_FusionDatas[i];
        TrueDataFrame &planeTrue = m_FusionOutput.m_FusionData.m_PlaneTrueDatas[i % m_FusionOutput.m_FusionData.m_PlaneTrueDatas.size()];
        frame.m_Pos = planeTrue.m_Pos + Position(300 * (i % 3), 300 * ((i + 1) % 3), 0);
    }
    for (int i = 0; i < m_FusionOutput.m_ControlDatas.size(); ++i)
    {
        m_FusionOutput.m_ControlDatas[i].m_FusionData = m_FusionOutput.m_FusionData;
    }
#endif
}

void CFusionPlaneDlg::AddControlDataAck(ControlDataAckPacket &packet)
{
    m_FusionOutput.m_FusionData.m_PlaneTrueDatas.push_back(packet.m_PlaneTrueData);
    m_FusionOutput.m_FusionData.m_MissileTrueDatas = packet.m_MissileTrueDatas;
    m_DataCenterSocket->SendFusionData(m_FusionOutput.m_FusionData);
}

void CFusionPlaneDlg::ResetSockets()
{
    m_Lock.Lock();
    CPlaneDlg::ResetSockets();
    for (int i = 0; i < m_PlaneSockets.size(); ++i)
    {
        m_PlaneSockets[i]->Close();
        delete m_PlaneSockets[i];
    }
    m_PlaneSockets.clear();
    m_Lock.Unlock();
}