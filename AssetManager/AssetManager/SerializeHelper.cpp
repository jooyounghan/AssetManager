#include "pch.h"
#include "SerializeHelper.h"

void SerializeHelper::SerializeString(const std::string& textIn, FILE* fileIn)
{
	size_t NameCount = textIn.size();
	fwrite(&NameCount, sizeof(size_t), 1, fileIn);
	fwrite(textIn.c_str(), sizeof(char), NameCount, fileIn);
}

std::string SerializeHelper::DeserializeString(FILE* fileIn)
{
	size_t NameCount;
	fread(&NameCount, sizeof(size_t), 1, fileIn);

	std::string result;
	result.resize(NameCount);
	fread(result.data(), sizeof(char), NameCount, fileIn);

	return result;
}