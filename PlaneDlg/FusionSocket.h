#pragma once

#include "PlaneSocket.h"

class CFusionPlaneDlg;

class FusionSocket :
    public PlaneSocket
{
public:
    FusionSocket(CFusionPlaneDlg *dlg);
    ~FusionSocket(void);

    void OnAccept(int nErrorCode);
    void OnClose(int nErrorCode);
    void Dispatch(int type, CArchive &ar);

    void SendKeyTarget(TrueDataFrame &keyTarget);

public:
    CFusionPlaneDlg *m_Dlg;
};

