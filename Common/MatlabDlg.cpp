#include "StdAfx.h"
#include "MatlabDlg.h"

using namespace MatlabHelper;

CMatlabDlg::CMatlabDlg()
: m_PlaneInput(NULL)
, m_TargetInput(NULL)
, m_Size(50)
, m_Thread(NULL)
, m_Engine(NULL)
{
}

CMatlabDlg::~CMatlabDlg(void)
{
    Stop();
}

void CMatlabDlg::Show()
{
    Run();
}

void CMatlabDlg::Hide()
{
    Stop();
}

void CMatlabDlg::Stop()
{
    m_ThreadLock.Lock();
    if (m_Thread)
    {
        if (!TerminateThread(m_Thread, 0))
        {
            AfxMessageBox(TEXT("Close thread WTF!"));
        }
        m_Thread = 0;
    }
    m_ThreadLock.Unlock();

    if (m_PlaneInput)
    {
        DestroyArray(m_PlaneInput);
        m_PlaneInput = NULL;
    }
    if (m_TargetInput)
    {
        DestroyArray(m_TargetInput);
        m_TargetInput = NULL;
    }

    /*
    if (m_Engine)
    {
        if (engClose(m_Engine))
        {
            AfxMessageBox(TEXT("Close engine WTF!"));
        }
        m_Engine = 0;
    }
    */
}

void CMatlabDlg::Reset()
{
    Stop();

    m_PlanePaths.clear();
    m_TargetPaths.clear();
}

void CMatlabDlg::Run()
{
    m_Lock.Lock();
    m_PlaneInput = CreateDoubleArray(m_PlanePaths.size(), m_Size * 3, (const unsigned char *)NULL, 0, 0);
    m_TargetInput = CreateDoubleArray(m_TargetPaths.size(), m_Size * 3, (const unsigned char *)NULL, 0, 0);

    double *data = mxGetPr(m_PlaneInput);
    for (int plane = 0; plane < m_PlanePaths.size(); ++plane)
    {
        for (int pos = 0; pos < m_PlanePaths[plane].size(); ++pos)
        {
            data[(pos * 3 + 0) * m_PlanePaths.size() + plane] = m_PlanePaths[plane][pos].X;
            data[(pos * 3 + 1) * m_PlanePaths.size() + plane] = m_PlanePaths[plane][pos].Y;
            data[(pos * 3 + 2) * m_PlanePaths.size() + plane] = m_PlanePaths[plane][pos].Z;
        }
    }
    data = mxGetPr(m_TargetInput);
    for (int target = 0; target < m_TargetPaths.size(); ++target)
    {
        for (int pos = 0; pos < m_TargetPaths[target].size(); ++pos)
        {
            data[(pos * 3 + 0) * m_TargetPaths.size() + target] = m_TargetPaths[target][pos].X;
            data[(pos * 3 + 1) * m_TargetPaths.size() + target] = m_TargetPaths[target][pos].Y;
            data[(pos * 3 + 2) * m_TargetPaths.size() + target] = m_TargetPaths[target][pos].Z;
        }
    }

    m_Thread = CreateThread(NULL,
        NULL,
        MatlabRun,
        (LPVOID)this,
        CREATE_SUSPENDED,
        0);
    SetThreadPriority(m_Thread, THREAD_PRIORITY_NORMAL);
    ResumeThread(m_Thread);
    m_Lock.Unlock();
}

DWORD WINAPI CMatlabDlg::MatlabRun(LPVOID lparam)
{
    CMatlabDlg *dlg = (CMatlabDlg *)lparam;
    dlg->m_ThreadLock.Lock();
    if (!(dlg->m_Engine = engOpen(NULL)))
    {
        AfxMessageBox(TEXT("��Matlab�������"));
    }
    else
    {
        if (engSetVisible(dlg->m_Engine, false))
        {
            AfxMessageBox(TEXT("Hide engine WTF!"));
        }
    }
    dlg->m_ThreadLock.Unlock();
    int result = 0;
    wchar_t buf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buf);
    wstring wsCurPath(buf);
    string curPath(wsCurPath.begin(), wsCurPath.end());
    string cmd = "cd " + curPath + "\\bin";
    result = engEvalString(dlg->m_Engine, cmd.c_str());
    if (result)
    {
        AfxMessageBox(TEXT("Cd engine WTF!"));
    }
    while (true)
    {
        result = engPutVariable(dlg->m_Engine, "plane_positions", dlg->m_PlaneInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 1 engine WTF!"));
        }
        result = engPutVariable(dlg->m_Engine, "target_positions", dlg->m_TargetInput);
        if (result)
        {
            AfxMessageBox(TEXT("Put var 2 engine WTF!"));
        }

        string cmd = "matlab_dialog(plane_positions, target_positions)";
        result = engEvalString(dlg->m_Engine, cmd.c_str());
        if (result)
        {
            AfxMessageBox(TEXT("Call func engine WTF!"));
        }
        if (result)
        {
            break;
        }
        Sleep(1);
    }

    /*
    if (engClose(dlg->m_Engine))
    {
        AfxMessageBox(TEXT("Close engine engine WTF!"));
    }
    dlg->m_Engine = 0;
    */
    dlg->m_Lock.Lock();
    dlg->m_Thread = 0;
    dlg->m_Lock.Unlock();
    return result;
}

void CMatlabDlg::AddPlane(Plane &plane)
{
    m_PlanePaths.push_back(Path());
}

void CMatlabDlg::AddPlaneData(int plane, Position pos)
{
    m_Lock.Lock();
    if (m_PlanePaths[plane].size() >= m_Size)
    {
        return;
    }
    if (m_PlaneInput)
    {
        double *data = mxGetPr(m_PlaneInput);
        data[(m_PlanePaths[plane].size() * 3 + 0) * m_PlanePaths.size() + plane] = pos.X;
        data[(m_PlanePaths[plane].size() * 3 + 1) * m_PlanePaths.size() + plane] = pos.Y;
        data[(m_PlanePaths[plane].size() * 3 + 2) * m_PlanePaths.size() + plane] = pos.Z;
    }
    m_PlanePaths[plane].push_back(pos);
    m_Lock.Unlock();
}

void CMatlabDlg::AddTarget(Target &target)
{
    m_TargetPaths.push_back(Path());
}

void CMatlabDlg::AddTargetData(int target, Position pos)
{
    m_Lock.Lock();
    if (m_TargetPaths[target].size() >= m_Size)
    {
        return;
    }
    if (m_TargetInput)
    {
        double *data = mxGetPr(m_TargetInput);
        data[(m_TargetPaths[target].size() * 3 + 0) * m_TargetPaths.size() + target] = pos.X;
        data[(m_TargetPaths[target].size() * 3 + 1) * m_TargetPaths.size() + target] = pos.Y;
        data[(m_TargetPaths[target].size() * 3 + 2) * m_TargetPaths.size() + target] = pos.Z;
    }
    m_TargetPaths[target].push_back(pos);
    m_Lock.Unlock();
}

void CMatlabDlg::SetSize(int size)
{
    m_Size = size;
}