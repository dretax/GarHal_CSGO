#pragma once
#include <process.h>
#include "..\common\bsp\BSPStructure.hpp"
#include "Entity.hpp"

namespace utils
{
    Entity CreateEntity(uint32_t Address);
    std::string GenerateStr(const int len);
    std::vector<std::string> Split(std::string s, std::string delimiter);
    bool AlignBonePairs(std::pair<Vector3, Vector3> pair, std::pair<ImVec2, ImVec2> &out);
    bool IsProcessElevated(HANDLE processHandle);
    hazedumper::Vector2 toWinPos(hazedumper::Vector2& screenPos, float width, float height);
}