#include "pch.h"
#include "StringHelper.h"

using namespace std;

string StringHelper::ConvertACPToUTF8(const string& acpString)
{
    int wideSize = MultiByteToWideChar(CP_ACP, 0, acpString.c_str(), -1, NULL, 0);
    wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_ACP, 0, acpString.c_str(), -1, &wideStr[0], wideSize);

    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
    string utf8Str(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Size, NULL, NULL);

    return utf8Str;
}

std::string StringHelper::ConvertWStringToACP(const std::wstring wString)
{
    int acpSize = WideCharToMultiByte(CP_UTF8, 0, wString.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string acpString(acpSize - 1, 0);
    WideCharToMultiByte(CP_ACP, 0, wString.c_str(), -1, &acpString[0], acpSize, nullptr, nullptr);
    return acpString;
}

wstring StringHelper::ConvertACPToWString(const string acpString)
{
    int wideSize = MultiByteToWideChar(CP_ACP, 0, acpString.c_str(), -1, NULL, 0);
    wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_ACP, 0, acpString.c_str(), -1, &wideStr[0], wideSize);
    return wideStr;
}
