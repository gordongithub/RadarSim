#include "StdAfx.h"
#include "FusionMatlabAlgo.h"

FusionMatlabAlgo::FusionMatlabAlgo(void)
: m_DllHandle(0)
, m_MatlabFunc(0)
{
}

FusionMatlabAlgo::FusionMatlabAlgo(const CString &name, const CString &dllFileName, const CString &funcName)
: FusionAlgo(name)
, m_DllFileName(dllFileName)
, m_FuncName(funcName)
, m_DllHandle(0)
, m_MatlabFunc(0)
{
}

FusionMatlabAlgo::~FusionMatlabAlgo(void)
{
    if (m_DllHandle)
    {
        FreeLibrary(m_DllHandle);
    }
}

bool FusionMatlabAlgo::Init()
{
    m_DllHandle = LoadLibrary(m_DllFileName);
    if (!m_DllHandle)
    {
        CString msg;
        msg.AppendFormat(TEXT("无法装载模块%s."), m_DllFileName);
        return false;
    }
    bool result = false;
    wstring wsFuncName(m_FuncName);
    string sFuncName(wsFuncName.begin(), wsFuncName.end());

    string initializeFuncName = sFuncName;
    initializeFuncName += InitializeFuncPrefix;

    InitializeFunc initializeFunc = (InitializeFunc)GetProcAddress(
        (HMODULE)m_DllHandle,
        initializeFuncName.c_str());

    if (!initializeFunc)
    {
        return false;
    }

    result = initializeFunc();

    if (!result)
    {
        return false;
    }

    string fullFuncName = sFuncName;
    fullFuncName[0] = toupper(fullFuncName[0]);
    fullFuncName = FuncPrefix + fullFuncName;
    m_MatlabFunc = (RealFunc)GetProcAddress(
        m_DllHandle,
        fullFuncName.c_str());
    if (!m_MatlabFunc)
    {
        return false;
    }

    return true;
}

void FusionMatlabAlgo::Output(CArchive &ar)
{
    ar << FusionAlgoTypeMatlab << m_DllFileName << m_FuncName;
    FusionAlgo::Output(ar);
}

void FusionMatlabAlgo::Input(CArchive &ar)
{
    ar >> m_DllFileName >> m_FuncName;
    FusionAlgo::Input(ar);
}

bool FusionMatlabAlgo::Run(const vector<NoiseDataPacket> &input, FusionOutput &output)
{
    if (!m_MatlabFunc)
    {
        CString msg;
        msg.AppendFormat(TEXT("算法%s地址错误."), m_Name);
        AfxMessageBox(msg);
        return false;
    }
    else
    {
        vector<Array *> inputList;
        Array *planeTrueDatas = CreateDoubleArray(input.size(), MATLAB_FUSION_TRUE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
        inputList.push_back(planeTrueDatas);
        Array *targetNoiseDatas = CreateDoubleArray(input.size() * input[0].m_TargetNoiseDatas.size(), MATLAB_FUSION_NOISE_DATA_SIZE, (const unsigned char *)NULL, 0, 0);
        inputList.push_back(targetNoiseDatas);
        Array *globalVar = CreateDoubleArray(PLANE_COUNT, TARGET_COUNT_MAX * MATLAB_GLOBAL_VAR_SIZE, (const unsigned char *)NULL, 0, 0);
        inputList.push_back(globalVar);

        double *p = mxGetPr(planeTrueDatas);
        for (int iPlane = 0; iPlane < input.size(); ++iPlane)
        {
            p[iPlane + 0 * input.size()] = input[iPlane].m_PlaneTrueData.m_Time;
            p[iPlane + 1 * input.size()] = input[iPlane].m_PlaneTrueData.m_Id;
            p[iPlane + 2 * input.size()] = input[iPlane].m_PlaneTrueData.m_Pos.X;
            p[iPlane + 3 * input.size()] = input[iPlane].m_PlaneTrueData.m_Pos.Y;
            p[iPlane + 4 * input.size()] = input[iPlane].m_PlaneTrueData.m_Pos.Z;
            p[iPlane + 5 * input.size()] = input[iPlane].m_PlaneTrueData.m_Vel.X;
            p[iPlane + 6 * input.size()] = input[iPlane].m_PlaneTrueData.m_Vel.Y;
            p[iPlane + 7 * input.size()] = input[iPlane].m_PlaneTrueData.m_Vel.Z;
            p[iPlane + 8 * input.size()] = input[iPlane].m_PlaneTrueData.m_Acc.X;
            p[iPlane + 9 * input.size()] = input[iPlane].m_PlaneTrueData.m_Acc.Y;
            p[iPlane + 10 * input.size()] = input[iPlane].m_PlaneTrueData.m_Acc.Z;
        }

        p = mxGetPr(targetNoiseDatas);
        for (int iPlane = 0; iPlane < input.size(); ++iPlane)
        {
            for (int iTarget = 0; iTarget < input[iPlane].m_TargetNoiseDatas.size(); ++iTarget)
            {
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 0 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Time;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 1 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Id;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 2 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Dis;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 3 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_DisVar;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 4 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Theta;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 5 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_ThetaVar;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 6 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_Phi;
                p[(iPlane * input[iPlane].m_TargetNoiseDatas.size() + iTarget) + 7 * (input.size() * input[iPlane].m_TargetNoiseDatas.size())] = input[iPlane].m_TargetNoiseDatas[iTarget].m_PhiVar;
            }
        }

        p = mxGetPr(globalVar);
        for (int plane = 0; plane < PLANE_COUNT; ++plane)
        {
            for (int target = 0; target < TARGET_COUNT_MAX; ++target)
            {
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 0) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G1;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 1) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G2;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 2) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G3;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 3) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G4;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 4) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G5;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 5) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G6;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 6) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G7;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 7) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G8;
                p[(target * MATLAB_GLOBAL_VAR_SIZE + 8) * PLANE_COUNT + plane] = g_GlobalVar[plane][target].m_G9;
            }
        }

        vector<Array *> outputList(3);
        bool result = m_MatlabFunc(3, &outputList[0], input.size(), &inputList[0]);
        if (!result)
        {
            DestroyArray(planeTrueDatas);
            DestroyArray(targetNoiseDatas);

            CString msg;
            msg.AppendFormat(TEXT("算法%s调用失败."), m_Name);

            return false;
        }

        Array *fusionDatas = outputList[0];
        Array *filterDatas = outputList[1];
        Array *controlDatas = outputList[2];
        Array *outputGlobalVar = outputList[3];

        p = mxGetPr(fusionDatas);
        int m = mxGetM(fusionDatas), n = mxGetN(fusionDatas);
        for (int iTarget = 0; iTarget < m; ++iTarget)
        {
            TrueDataFrame fusionData;
            fusionData.m_Time = p[iTarget + 0 * m];
            fusionData.m_Id = p[iTarget + 1 * m];
            fusionData.m_Pos.X = p[iTarget + 2 * m];
            fusionData.m_Pos.Y = p[iTarget + 3 * m];
            fusionData.m_Pos.Z = p[iTarget + 4 * m];
            fusionData.m_Vel.X = p[iTarget + 5 * m];
            fusionData.m_Vel.Y = p[iTarget + 6 * m];
            fusionData.m_Vel.Z = p[iTarget + 7 * m];
            fusionData.m_Acc.X = p[iTarget + 8 * m];
            fusionData.m_Acc.Y = p[iTarget + 9 * m];
            fusionData.m_Acc.Z = p[iTarget + 10 * m];
            output.m_FusionData.m_FusionDatas.push_back(fusionData);
        }

        p = mxGetPr(filterDatas);
        m = mxGetM(filterDatas);
        n = mxGetN(filterDatas);
        for (int iTarget = 0; iTarget < m; ++iTarget)
        {
            TrueDataFrame filterData;
            filterData.m_Time = p[iTarget + 0 * m];
            filterData.m_Id = p[iTarget + 1 * m];
            filterData.m_Pos.X = p[iTarget + 2 * m];
            filterData.m_Pos.Y = p[iTarget + 3 * m];
            filterData.m_Pos.Z = p[iTarget + 4 * m];
            filterData.m_Vel.X = p[iTarget + 5 * m];
            filterData.m_Vel.Y = p[iTarget + 6 * m];
            filterData.m_Vel.Z = p[iTarget + 7 * m];
            filterData.m_Acc.X = p[iTarget + 8 * m];
            filterData.m_Acc.Y = p[iTarget + 9 * m];
            filterData.m_Acc.Z = p[iTarget + 10 * m];
            output.m_FusionData.m_FilterDatas.push_back(filterData);
        }

        for (int iPlane = 0; iPlane < input.size(); ++iPlane)
        {
            output.m_FusionData.m_NoiseDatas.push_back(input[iPlane]);
        }

        p = mxGetPr(controlDatas);
        m = mxGetM(controlDatas);
        n = mxGetN(controlDatas);
        for (int iPlane = 0; iPlane < m; ++iPlane)
        {
            ControlDataPacket packet;
            packet.m_ControlData.m_Time = p[iPlane + 0 * m];
            packet.m_ControlData.m_Id = p[iPlane + 1 * m];
            output.m_ControlDatas.push_back(packet);
        }

        p = mxGetPr(outputGlobalVar);
        m = mxGetM(outputGlobalVar);
        n = mxGetN(outputGlobalVar);
        for (int plane = 0; plane < m; ++plane)
        {
            for (int target = 0; target < TARGET_COUNT_MAX; ++target)
            {
                g_GlobalVar[plane][target].m_G1 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 0) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G2 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 1) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G3 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 2) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G4 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 3) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G5 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 4) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G6 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 5) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G7 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 6) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G8 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 7) * PLANE_COUNT + plane];
                g_GlobalVar[plane][target].m_G9 = p[(target * MATLAB_GLOBAL_VAR_SIZE + 8) * PLANE_COUNT + plane];
            }
        }

        DestroyArray(planeTrueDatas);
        DestroyArray(targetNoiseDatas);
        DestroyArray(globalVar);

        DestroyArray(fusionDatas);
        DestroyArray(filterDatas);
        DestroyArray(controlDatas);
        DestroyArray(outputGlobalVar);

        return true;
    }
}
