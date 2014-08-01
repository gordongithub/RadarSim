
// AttackPlaneDlg.h : ͷ�ļ�
//

#pragma once
#include "PlaneDlg.h"

#include <afxmt.h>
#include <map>

using namespace std;

// CAttackPlaneDlg �Ի���
class CAttackPlaneDlg : public CPlaneDlg
{
// ����
public:
	CAttackPlaneDlg(LPCWSTR title, CWnd* pParent = NULL);	// ��׼���캯��
	~CAttackPlaneDlg();	// ��׼���캯��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    virtual void ResetCtrls();

    virtual void ConnectDataCenter();
    virtual void SendNoiseData(NoiseDataPacket &packet);
    virtual void SetNaviAlgo(NaviAlgo *algo);
    NaviAlgo *m_NaviAlgo;
    void DoNavi(const ControlDataPacket &packet);
    NaviOutput m_NaviOutput;

    virtual void AddTrueData(TrueDataPacket &packet);
    virtual void AddControlData(ControlDataPacket &packet);

    vector<TargetState> m_TargetStates;
};
