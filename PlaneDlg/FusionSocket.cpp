#include "StdAfx.h"
#include "FusionSocket.h"
#include "Resource.h"
#include "FusionPlaneDlg.h"
#include "FusionLocalAlgo.h"
#include "FusionVcAlgo.h"

FusionSocket::FusionSocket(CFusionPlaneDlg *dlg)
: m_Dlg(dlg)
{
}

FusionSocket::~FusionSocket(void)
{
}

void FusionSocket::OnAccept(int nErrorCode)
{
    m_Dlg->AddPlaneSocket();
    CSocket::OnAccept(nErrorCode);
}

void FusionSocket::OnReceive(int nErrorCode)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::load);

    int type;
    ar >> type;
    switch (type)
    {
    case PacketTypeNoiseData:
        {
            NoiseDataPacket packet;
            ar >> packet;
            m_Dlg->AddNoiseData(packet);
        }
        break;
    case PacketTypeFusionAlgo:
        {
            int type;
            ar >> type;
            switch (type)
            {
            case FusionAlgoTypeLocal:
                {
                    FusionAlgo *algo = new FusionLocalAlgo;
                    ar >> *algo;
                    m_Dlg->SetFusionAlgo(algo);
                }
                break;
            case FusionAlgoTypeVc:
                {
                    FusionAlgo *algo = new FusionVcAlgo;
                    ar >> *algo;
                    m_Dlg->SetFusionAlgo(algo);
                }
                break;
            default:
                CString msg;
                msg.AppendFormat(TEXT("δ֪�ں��㷨����%d."), type);
                AfxMessageBox(msg);
                break;
            }
        }
        break;
    default:
        AfxMessageBox(TEXT("δ֪���ݰ�����"));
        break;
    }

    ar.Flush();
    AsyncSelect(FD_READ);
    CSocket::OnReceive(nErrorCode);
}

void FusionSocket::OnClose(int nErrorCode)
{
    AfxMessageBox(TEXT("���һ������ӶϿ�"));
    m_Dlg->ResetSockets();
    m_Dlg->ConnectDataCenter();
    CSocket::OnClose(nErrorCode);
}