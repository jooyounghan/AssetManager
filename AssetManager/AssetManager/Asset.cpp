#include "pch.h"
#include "Asset.h"

AAsset::AAsset(const std::string& assetNameIn)
	: m_assetName(assetNameIn)
{
}

AAsset::~AAsset()
{
}

void AAsset::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeString(m_assetName, fileIn);
}

void AAsset::Deserialize(FILE* fileIn)
{
	m_assetName = DeserializeHelper::DeserializeString(fileIn);
}
