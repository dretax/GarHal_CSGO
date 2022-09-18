#include "utils.hpp"

#include "Engine.hpp"
#include "..\common\bsp\BSPStructure.hpp"


Entity utils::CreateEntity(uint32_t Address)
{
    if (Address > 0)
    {
        Entity entity(Address);
        return entity;
    }
	
    Entity dummy(0);
    return dummy;
}

std::string utils::GenerateStr(const int len)
{
    std::string str;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL) * _getpid());

    for (int i = 0; i < len; ++i) 
    {
        str += alphanum[rand() % (sizeof(alphanum) - 1)];
    }


    return str;
}

std::vector<std::string> utils::Split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) 
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

bool utils::AlignBonePairs(std::pair<Vector3, Vector3> pair, std::pair<ImVec2, ImVec2> &out)
{
    Vector3 result;
    Vector3 result2;
    if (engine::worldToScreen(pair.first, result) && engine::worldToScreen(pair.second, result2))
    {
        out = std::make_pair(ImVec2(result.at(0), result.at(1)), ImVec2(result2.at(0), result2.at(1)));
        return true;
    }

    return false;
} 

bool utils::IsProcessElevated(HANDLE processHandle)
{
    BOOL fIsElevated = FALSE;
    HANDLE hToken = NULL;
    TOKEN_ELEVATION elevation;
    DWORD dwSize;

    if (!OpenProcessToken(processHandle, TOKEN_QUERY, &hToken))
    {
        goto Cleanup;
    }

    if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
    {
        goto Cleanup;
    }

    fIsElevated = elevation.TokenIsElevated;

    Cleanup:
    if (hToken)
    {
        CloseHandle(hToken);
        hToken = NULL;
    }
    
    return fIsElevated;
}


hazedumper::Vector2 utils::toWinPos(hazedumper::Vector2& screenPos, float width, float height)
{
    return hazedumper::Vector2((screenPos.at(0) + width) / 2, (-screenPos.at(1) + height) / 2);
}