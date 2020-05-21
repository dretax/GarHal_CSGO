#pragma once
#include <minwindef.h>
#include "data.hpp"

class CUserCmd {
public:
    DWORD Vft;
    DWORD CmdNumber;
    DWORD TickCount;
    Vector3 ViewAngles;
    Vector3 AimDirection;
    FLOAT Forwardmove;
    FLOAT Sidemove;
    FLOAT Upmove;
    DWORD Buttons;
    BYTE Impulse;

private:
    BYTE __pad0x01[0x03];

public:
    DWORD WeaponSelect;
    DWORD WeaponSubtype;
    DWORD RandomSeed;
    WORD MouseDx;
    WORD MouseDy;
    BOOLEAN HasBeenPredicted;

private:
    BYTE __pad0x02[0x1B];
};
class CVerifiedUserCmd {
public:
    CUserCmd Command;
    DWORD CRC;
};