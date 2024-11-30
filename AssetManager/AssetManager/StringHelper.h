#pragma once
#include <string>

class StringHelper
{
public:
	static std::string ConvertACPToUTF8(const std::string& acpString);
	static std::string ConvertWStringToACP(const std::wstring wString);
	static std::wstring ConvertACPToWString(const std::string acpString);
};