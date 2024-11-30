#include "pch.h"
#include "AssetWrapper.h"

void AssetWrapper::SetAssetType(const EAssetType& assetType) { m_assetType = assetType; }

void AssetWrapper::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeElement(m_assetType, fileIn);
}

void AssetWrapper::Deserialize(FILE* fileIn)
{
	m_assetType = DeserializeHelper::DeserializeElement<EAssetType>(fileIn);
}