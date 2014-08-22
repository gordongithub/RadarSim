// FusionVcAlgoTest1.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FusionVcAlgoTest1.h"
#include "FusionAlgo.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CFusionVcAlgoTest1App

BEGIN_MESSAGE_MAP(CFusionVcAlgoTest1App, CWinApp)
END_MESSAGE_MAP()


// CFusionVcAlgoTest1App ����

CFusionVcAlgoTest1App::CFusionVcAlgoTest1App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFusionVcAlgoTest1App ����

CFusionVcAlgoTest1App theApp;


// CFusionVcAlgoTest1App ��ʼ��

BOOL CFusionVcAlgoTest1App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

__declspec(dllimport) GlobalVarFrame *g_GlobalVar;

extern "C" __declspec(dllexport) bool FusionVcAlgoTest1(const FusionInput &input, FusionOutput &output)
{
    // This is how to use global var.
    g_GlobalVar[0].m_G[0] = 0;
    g_GlobalVar[0].m_G[1] = 0;
    const vector<NoiseDataPacket> &noiseDatas = input.m_NoiseDataPackets;
    int interval = input.m_Interval;

    int nTargets = noiseDatas.front().m_TargetNoiseDatas.size();
    int nPlanes = noiseDatas.size();
    for (int iTarget = 0; iTarget < nTargets; ++iTarget)
    {
        TrueDataFrame frame;
        assert(noiseDatas.front().m_TargetNoiseDatas.size() > iTarget);
        frame.m_Time = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Time;
        frame.m_Id = noiseDatas.front().m_TargetNoiseDatas[iTarget].m_Id;
        for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
        {
            // frame += noiseDatas[iPlane].m_TargetNoiseDatas[iTarget];
        }
        // frame /= nPlanes;
        output.m_FusionData.m_FusionDatas.push_back(frame);
        // frame = noiseDatas.front().m_TargetNoiseDatas[iTarget];
        output.m_FusionData.m_FilterDatas.push_back(frame);
    }
    for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
    {
        output.m_FusionData.m_NoiseDatas.push_back(noiseDatas[iPlane]);
    }

    for (int iPlane = 0; iPlane < nPlanes; ++iPlane)
    {
        ControlDataPacket packet;
        packet.m_ControlData.m_Time = noiseDatas[iPlane].m_PlaneTrueData.m_Time;
        packet.m_ControlData.m_Id = noiseDatas[iPlane].m_PlaneTrueData.m_Id;
        output.m_ControlDatas.push_back(packet);
    }
    return true;
}
