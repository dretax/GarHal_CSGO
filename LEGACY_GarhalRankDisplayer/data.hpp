#pragma once
#include <windows.h>
#include "kernelinterface.hpp"

inline DWORD ProcessId, ClientAddress, EngineAddress;
inline KeInterface Driver = NULL;

inline const char* Ranks[] =
{
        "UnRanked",
        "Silver I",
        "Silver II",
        "Silver III",
        "Silver IV",
        "Silver Elite",
        "Silver Elite Master",

        "Gold Nova I",
        "Gold Nova II",
        "Gold Nova III",
        "Gold Nova Master",
        "Master Guardian I",
        "Master Guardian II",

        "Master Guardian Elite",
        "Distinguished Master Guardian",
        "Legendary Eagle",
        "Legendary Eagle Master",
        "Supreme Master First Class",
        "The Global Elite"
};