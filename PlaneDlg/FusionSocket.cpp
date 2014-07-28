#include "StdAfx.h"
#include "FusionSocket.h"
#include "Resource.h"
#include "PlaneDlg.h"

FusionSocket::FusionSocket(CPlaneDlg *dlg)
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
    case PacketTypeControlData:
        {
            ControlDataPacket packet;
            ar >> packet;
            m_Dlg->AddControlData(packet);
        }
        break;
    case PacketTypeControlDataAck:
        {
            ControlDataAckPacket packet;
            ar >> packet;
            m_Dlg->AddControlDataAck(packet);
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
    AfxMessageBox(TEXT("���ںϻ������ӶϿ�"));
    m_Dlg->ResetSockets();
    m_Dlg->ConnectDataCenter();
    CSocket::OnClose(nErrorCode);
}

void FusionSocket::SendNoiseData(NoiseDataPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeNoiseData << packet;
    ar.Flush();
}

void FusionSocket::SendControlDataAck(ControlDataAckPacket &packet)
{
    CSocketFile file(this);
    CArchive ar(&file, CArchive::store);
    ar << PacketTypeControlDataAck << packet;
    ar.Flush();
}
