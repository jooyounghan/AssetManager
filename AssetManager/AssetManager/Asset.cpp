#include "pch.h"
#include "Asset.h"

AAsset::AAsset(const std::string& assetPathIn, const std::string& assetNameIn)
	: m_assetPath(assetPathIn), m_assetName(assetNameIn)
{
}

AAsset::~AAsset()
{
}

void AAsset::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeString(m_assetPath, fileIn);
	SerializeHelper::SerializeString(m_assetName, fileIn);
}

void AAsset::Deserialize(FILE* fileIn)
{
	m_assetPath = DeserializeHelper::DeserializeString(fileIn);
	m_assetName = DeserializeHelper::DeserializeString(fileIn);
}
