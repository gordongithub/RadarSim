#include "StdAfx.h"
#include "NaviLocalAlgo.h"
#include <assert.h>

NaviLocalAlgo::NaviLocalAlgo()
: m_Type(NaviLocalAlgoTypeTest1)
{
}

NaviLocalAlgo::NaviLocalAlgo(const CString &name, NaviLocalAlgoType type)
: NaviAlgo(name)
, m_Type(type)
{
}

bool NaviLocalAlgo::Init()
{
    switch (m_Type)
    {
    case NaviLocalAlgoTypeTest1:
        m_Func = &NaviLocalAlgoTest1;
        break;
    case NaviLocalAlgoTypeTest2:
        m_Func = &NaviLocalAlgoTest2;
        break;
    default:
        CString msg;
        msg.AppendFormat(TEXT("δ֪���ص����㷨%d."), m_Type);
        AfxMessageBox(msg);
        return false;
    }
    return true;
}

void NaviLocalAlgo::Output(CArchive &ar)
{
    ar << NaviAlgoTypeLocal << m_Type;
    NaviAlgo::Output(ar);
}

void NaviLocalAlgo::Input(CArchive &ar)
{
    int type;
    ar >> type;
    m_Type = (NaviLocalAlgoType)type;
    NaviAlgo::Input(ar);
}

bool NaviLocalAlgoTest1(const NaviInput &input, NaviOutput &output)
{
    // How to get interval.
    int interval = input.m_GlobalData.m_Interval;

    // How to use global var.
    g_GlobalVar[0][0].m_G1 = 0;
    g_GlobalVar[0][0].m_G2 = 0;

    output.m_TrueData.m_Pos = Position(10000, 40000, 0);
    return true;
}

bool NaviLocalAlgoTest2(const NaviInput &input, NaviOutput &output)
{
    // How to get interval.
    int interval = input.m_GlobalData.m_Interval;

    // How to use global var.
    g_GlobalVar[0][0].m_G1 = 0;
    g_GlobalVar[0][0].m_G2 = 0;

    output.m_TrueData.m_Pos = Position(40000, 10000, 0);
    return true;
}