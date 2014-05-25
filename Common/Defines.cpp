#include "stdafx.h"

#include "Defines.h"
#include "DataPacket.h"
#include <mclmcrrt.h>

Color __declspec(dllexport) TargetColors[] =
{
    Color::Red,
    Color::Orange,
    Color::Yellow,
    Color::Green,
    Color::Cyan,
    Color::Blue,
    Color::Purple,
};

CString __declspec(dllexport) TargetColorNames[] =
{
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
    TEXT("��ɫ"),
};

Image __declspec(dllexport) *StateMapBackgrounds[];

CString __declspec(dllexport) StateMapBackgroundNames[] = 
{
    TEXT("��ͼ0"),
    TEXT("��ͼ1"),
    TEXT("��ͼ2"),
    TEXT("��ͼ3"),
    TEXT("��ͼ4"),
};

Image __declspec(dllexport) *TargetTypeImages[] =
{
    Image::FromFile(TEXT("Heli.ico")),
    Image::FromFile(TEXT("Fighter.ico")),
    Image::FromFile(TEXT("Aew.ico")),
    Image::FromFile(TEXT("Bomber.ico")),
    Image::FromFile(TEXT("Shipboard.ico")),
    Image::FromFile(TEXT("Tank.ico")),
    Image::FromFile(TEXT("Panzer.ico")),
    Image::FromFile(TEXT("Missile.ico")),
};

CString __declspec(dllexport) TargetTypeNames[] =
{
    TEXT("ֱ����"),
    TEXT("�ߵл�"),
    TEXT("Ԥ����"),
    TEXT("��ը��"),
    TEXT("���ػ�"),
    TEXT("̹��"),
    TEXT("װ�׳�"),
    TEXT("����"),
};

CString __declspec(dllexport) TargetMoveTypeNames[] =
{
    TEXT("����ֱ���˶�"),
    TEXT("�ȼ���ֱ���˶�"),
    TEXT("����Բ���˶�"),
};

CString __declspec(dllexport) NoiseTypeNames[] =
{
    TEXT("������"),
    TEXT("ɫ����"),
    TEXT("��������"),
};

CString __declspec(dllexport) SensorIdNames[] =
{
    TEXT("�״�"),
    TEXT("ESM"),
    TEXT("����"),
};

CArchive & operator << (CArchive &ar, Position &pos)
{
    ar << pos.X << pos.Y << pos.Z;

    return ar;
}

CArchive & operator >> (CArchive &ar, Position &pos)
{
    ar >> pos.X >> pos.Y >> pos.Z;

    return ar;
}

wofstream & operator << (wofstream &os, Position &pos)
{
    os << pos.X << TEXT(" ") << pos.Y << TEXT(" ") << pos.Z;

    return os;
}

wifstream & operator >> (wifstream &is, Position &pos)
{
    is >> pos.X >> pos.Y >> pos.Z;

    return is;
}

CString __declspec(dllexport) ConfigFileName(TEXT("config.ini"));

void __declspec(dllexport) GlobalInit()
{
    srand((unsigned int)time(NULL));

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    StateMapBackgrounds[StateMapBackground0] = Image::FromFile(TEXT("StateMapBackground0.bmp"));
    StateMapBackgrounds[StateMapBackground1] = Image::FromFile(TEXT("StateMapBackground1.bmp"));
    StateMapBackgrounds[StateMapBackground2] = Image::FromFile(TEXT("StateMapBackground2.bmp"));
    StateMapBackgrounds[StateMapBackground3] = Image::FromFile(TEXT("StateMapBackground3.bmp"));
    StateMapBackgrounds[StateMapBackground4] = Image::FromFile(TEXT("StateMapBackground4.bmp"));

    TargetTypeImages[TargetTypeHeli] = Image::FromFile(TEXT("Heli.ico"));
    TargetTypeImages[TargetTypeFighter] = Image::FromFile(TEXT("Fighter.ico"));
    TargetTypeImages[TargetTypeAew] = Image::FromFile(TEXT("Aew.ico"));
    TargetTypeImages[TargetTypeBomber] = Image::FromFile(TEXT("Bomber.ico"));
    TargetTypeImages[TargetTypeShipboard] = Image::FromFile(TEXT("Shipboard.ico"));
    TargetTypeImages[TargetTypeTank] = Image::FromFile(TEXT("Tank.ico"));
    TargetTypeImages[TargetTypePanzer] = Image::FromFile(TEXT("Panzer.ico"));
    TargetTypeImages[TargetTypeMissile] = Image::FromFile(TEXT("Missile.ico"));

    typedef GlobalVarFrame *GlobalVarFrameP;
    g_GlobalVar = new GlobalVarFrameP[PLANE_COUNT];
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        g_GlobalVar[i] = new GlobalVarFrame[TARGET_COUNT_MAX];
    }

    mclInitializeApplication(NULL, 0);
}

void __declspec(dllexport) GlobalShut()
{
    for (int i = 0; i < PLANE_COUNT; ++i)
    {
        delete[] g_GlobalVar;
    }
    delete[] g_GlobalVar;

    for (int i = StateMapBackground0; i < StateMapBackgroundLast; ++i)
    {
        delete StateMapBackgrounds[i];
    }
    for (int i = TargetTypeHeli; i < TargetTypeLast; ++i)
    {
        delete TargetTypeImages[i];
    }
    ULONG_PTR gdiplusToken = NULL; 
    GdiplusShutdown(gdiplusToken);

    mclTerminateApplication();
}
