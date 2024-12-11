#include "pch.h"
#include "Asset.h"

using namespace std;

string AAsset::AssetExtension = ".Asset";

AAsset::AAsset(const string& assetName)
	: m_assetName(assetName)
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

void AAsset::SerializeAssetName(const AAsset* asset, FILE* fileIn) const
{
	SerializeHelper::SerializeString(
		asset != nullptr ? asset->GetAssetName() : "",
		fileIn
	);
}