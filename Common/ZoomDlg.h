#pragma once

#include <afxwin.h>

#include "Common.h"
#include "CommonDlg.h"

// CZoomDlg �Ի���

class __declspec(dllexport) CZoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CZoomDlg)

public:
	CZoomDlg(CCommonDlg *dlg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZoomDlg();

    static void CreateDlg(CZoomDlg &dlg);

// �Ի�������
	enum { IDD = IDD_ZOOM_DLG };

protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnClose();

public:
    CCommonDlg *m_Dlg;
    Image *m_Image;
    bool m_Initialized;

    void DrawTarget();
};
