
// PlaneT.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPlaneTApp:
// �йش����ʵ�֣������ PlaneT.cpp
//

class CPlaneTApp : public CWinAppEx
{
public:
	CPlaneTApp();
    virtual ~CPlaneTApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPlaneTApp theApp;