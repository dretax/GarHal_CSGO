#pragma once
#include <minwindef.h>

class CInput {
private:
    BYTE __pad0x01[0xF1];
public:
    DWORD Commands;
    DWORD VerifiedCommands;
};
