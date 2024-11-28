#include "pch.h"
#include "StringHelper.h"

using namespace std;

string StringHelper::ConvertACPToUTF8(const string& ACPString)
{
    int wideSize = MultiByteToWideChar(CP_ACP, 0, ACPString.c_str(), -1, NULL, 0);
    wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_ACP, 0, ACPString.c_str(), -1, &wideStr[0], wideSize);

    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, NULL, 0, NULL, NULL);
    string utf8Str(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Size, NULL, NULL);

    return utf8Str;
}

wstring StringHelper::ConvertACPToWString(const string ACPString)
{
    int wideSize = MultiByteToWideChar(CP_ACP, 0, ACPString.c_str(), -1, NULL, 0);
    wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_ACP, 0, ACPString.c_str(), -1, &wideStr[0], wideSize);
    return wideStr;
}