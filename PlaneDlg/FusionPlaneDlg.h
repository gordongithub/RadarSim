
// FusionPlaneDlg.h : ͷ�ļ�
//

#pragma once
#include "PlaneDlg.h"

#include <afxmt.h>
#include <map>

#include "FusionSocket.h"
#include "FusionAlgo.h"

using namespace std;

// CFusionPlaneDlg �Ի���
class CFusionPlaneDlg : public CPlaneDlg
{
// ����
public:
	CFusionPlaneDlg(LPCWSTR title
        , bool hasSensor1
        , CString sensor1Title
        , bool hasSensor2
        , CString sensor2Title
        , bool hasStateMap
        , bool hasDataList = true
        , CWnd* pParent = NULL);	// ��׼���캯��
    ~CFusionPlaneDlg();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    virtual void CreateDataCenterSocket();
    virtual void ConnectFusion(const CString &addr, int port);
    virtual void SendPlaneType();
    virtual void AddPlaneSocket();
    virtual void AddNoiseData(NoiseDataPacket &packet);
    virtual void SetFusionAlgo(FusionAlgo *algo);
    virtual void DoFusion();
    virtual void SendNoiseDatas(TrueDataPacket &packet);
    virtual void ResetSockets();

    FusionSocket *m_FusionSocket;
    vector<FusionSocket *> m_PlaneSockets;
    CCriticalSection m_Lock;
    FusionAlgo *m_FusionAlgo;
    map<SensorId, NoiseDataPacket> m_NoiseDatas;
    FusionDataPacket m_FusionDataPacket;
};
