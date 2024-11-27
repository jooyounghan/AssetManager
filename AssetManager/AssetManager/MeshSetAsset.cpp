#include "pch.h"
#include "MeshSetAsset.h"

using namespace std;

MeshSetAsset::MeshSetAsset(const std::string& assetName)
	: AAsset(assetName)
{
}

MeshSetAsset::~MeshSetAsset()
{
}

void MeshSetAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_meshAssetNamesToModelMaterialNames, fileIn);

	for (auto& meshAssetNamesToModelMaterialName : m_meshAssetNamesToModelMaterialNames)
	{
		const string& meshAssetName = meshAssetNamesToModelMaterialName.first;
		const string& modelMaterialAssetName = meshAssetNamesToModelMaterialName.second;

		SerializeHelper::SerializeString(meshAssetName, fileIn);
		SerializeHelper::SerializeString(modelMaterialAssetName, fileIn);
	}
}

void MeshSetAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	const size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerSize; ++idx)
	{
		const string meshAssetName = DeserializeHelper::DeserializeString(fileIn);
		const string modelMaterialAssetName = DeserializeHelper::DeserializeString(fileIn);
		m_meshAssetNamesToModelMaterialNames.emplace(meshAssetName, modelMaterialAssetName);
	}
}
