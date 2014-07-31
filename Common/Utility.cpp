#include "stdafx.h"
#include "Utility.h"

#include <math.h>

namespace Utility
{
    double Distance(const Position &rel)
    {
        return sqrt(pow(rel.X, 2) + pow(rel.Y, 2) + pow(rel.Z, 2));
    }

    double Theta(const Position &rel)
    {
        return atan(rel.Y / rel.X) * 57.2957795785523;
    }

    double Phi(const Position &rel)
    {
        return asin(rel.Z / Distance(rel)) * 57.2957795785523;
    }

    double Distance(const Position &src, const Position &dst)
    {
        Position rel = dst - src;
        return Distance(rel);
    }

    double Theta(const Position &src, const Position &dst)
    {
        Position rel = dst - src;
        return Theta(rel);
    }

    double Phi(const Position &src, const Position &dst)
    {
        Position rel = dst - src;
        return Phi(rel);
    }

    Position Rel(double dis, double theta, double phi)
    {
        double z = sin(phi / 57.2957795785523) * dis;
        double ydx = tan(theta / 57.2957795785523);
        double x = sqrt((pow(dis, 2) - pow(z, 2)) / (1 + pow(ydx, 2)));
        double y = x * ydx;
        return Position(x, y, z);
    }

    double WhiteNoise(double value, double var)
    {
        return value + (double)rand() / (double)RAND_MAX * var;
    }
    double ColorNoise(double value, double var)
    {
        // �������ɫ����.
        return value + (double)rand() / (double)RAND_MAX * var;
    }
    double MultNoise(double value, double var)
    {
        // ������ӳ�������.
        return value + (double)rand() / (double)RAND_MAX * var;
    }

    void CheckMissileHit(vector<Missile> &missiles, vector<TrueDataFrame> &targetTrueDatas)
    {
        for (int i = 0; i < missiles.size(); ++i)
        {
            Missile &miss = missiles[i];
            if (miss.m_State == TargetStateDestroyed)
            {
                continue;
            }
            if (miss.m_State == TargetStateExploding)
            {
                miss.m_State = TargetStateDestroyed;
                continue;
            }
            for (int j = 0; j < targetTrueDatas.size(); ++j)
            {
                TrueDataFrame &frame = targetTrueDatas[j];
                if (frame.m_State == TargetStateDestroyed)
                {
                    continue;
                }
                if (frame.m_State == TargetStateExploding)
                {
                    frame.m_State = TargetStateDestroyed;
                    continue;
                }
                if (Utility::Distance(frame.m_Pos - miss.m_Position) <= MISSILE_HIT_THRESHOLD)
                {
                    miss.m_State = TargetStateExploding;
                    frame.m_State = TargetStateExploding;
                }
            }
        }
    }

    void CheckMissileHit(vector<Missile> &missiles, vector<Target *> &targetClients)
    {
        for (int i = 0; i < missiles.size(); ++i)
        {
            Missile &miss = missiles[i];
            if (miss.m_State == TargetStateDestroyed)
            {
                continue;
            }
            if (miss.m_State == TargetStateExploding)
            {
                miss.m_State = TargetStateDestroyed;
                continue;
            }
            for (int j = 0; j < targetClients.size(); ++j)
            {
                Target &tar = *targetClients[j];
                if (tar.m_State == TargetStateDestroyed)
                {
                    continue;
                }
                if (tar.m_State == TargetStateExploding)
                {
                    tar.m_State = TargetStateDestroyed;
                    continue;
                }
                if (Utility::Distance(tar.m_Position - miss.m_Position) <= MISSILE_HIT_THRESHOLD)
                {
                    miss.m_State = TargetStateExploding;
                    tar.m_State = TargetStateExploding;
                }
            }
        }
    }
};