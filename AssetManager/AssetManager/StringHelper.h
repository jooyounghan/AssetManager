#pragma once
#include <string>

class StringHelper
{
public:
	static std::string ConvertACPToUTF8(const std::string& ACPString);
	static std::wstring ConvertACPToWString(const std::string ACPString);
};