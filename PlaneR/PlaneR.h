
// PlaneR.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPlaneRApp:
// �йش����ʵ�֣������ PlaneR.cpp
//

class CPlaneRApp : public CWinAppEx
{
public:
	CPlaneRApp();
    virtual ~CPlaneRApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPlaneRApp theApp;