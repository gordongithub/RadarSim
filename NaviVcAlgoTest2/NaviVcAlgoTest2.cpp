// NaviVcAlgoTest2.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "NaviVcAlgoTest2.h"
#include "NaviAlgo.h"

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

// CNaviVcAlgoTest2App

BEGIN_MESSAGE_MAP(CNaviVcAlgoTest2App, CWinApp)
END_MESSAGE_MAP()


// CNaviVcAlgoTest2App ����

CNaviVcAlgoTest2App::CNaviVcAlgoTest2App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNaviVcAlgoTest2App ����

CNaviVcAlgoTest2App theApp;


// CNaviVcAlgoTest2App ��ʼ��

BOOL CNaviVcAlgoTest2App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

__declspec(dllimport) GlobalVarFrame *g_GlobalVar;

extern "C" __declspec(dllexport) bool NaviVcAlgoTest2(const NaviInput &input, NaviOutput &output)
{
    // How to get interval.
    int interval = input.m_Interval;

    // How to use global var.
    g_GlobalVar[0].m_G[0] = 0;
    g_GlobalVar[0].m_G[1] = 0;

    output.m_PlaneTrueData.m_Pos = Position(30000, 20000, 0);
    return true;
}
