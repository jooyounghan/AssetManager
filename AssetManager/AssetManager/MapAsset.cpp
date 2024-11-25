#include "pch.h"
#include "MapAsset.h"

using namespace std;
using namespace DirectX;

MapAsset::MapAsset(const std::string& assetNameIn)
	: AAsset(assetNameIn)
{
}

MapAsset::~MapAsset()
{
}

void MapAsset::UpdateIBLMaterialAsset(IIBLMaterialProvider& provider)
{
	m_iblMaterial = provider.GetIBLMaterialAsset(m_iblMaterialName);
}

void MapAsset::UpdateStaticMeshAssets(IStaticMeshProvider& provider)
{
	m_assetNameToAssets.clear();
	for (const auto& assetNameToTransformation : m_assetNameToTransformations)
	{
		const string& assetName = assetNameToTransformation.first;
		m_assetNameToAssets.emplace(assetName, provider.GetStaticMeshAsset(assetName));
	}
}

void MapAsset::SetIBLMaterialAsset(const std::string& iblmaterialAssetNameIn, IIBLMaterialProvider& provider)
{
	m_iblMaterialName = iblmaterialAssetNameIn;
	UpdateIBLMaterialAsset(provider);
	m_isModified = true;
}

void MapAsset::AddStaticMesh(
	const std::string& assetName, 
	const DirectX::XMMATRIX& transformation,
	IStaticMeshProvider& provider
)
{
	if (m_assetNameToAssets.find(assetName) != m_assetNameToAssets.end())
	{
		m_assetNameToAssets.emplace(assetName, provider.GetStaticMeshAsset(assetName));
	}
	m_assetNameToTransformations[assetName].push_back(transformation);
}

void MapAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeString(m_iblMaterialName, fileIn);

	SerializeHelper::SerializeContainerSize(m_assetNameToTransformations, fileIn);
	for (const auto& assetNameToTransformation : m_assetNameToTransformations)
	{
		SerializeHelper::SerializeString(assetNameToTransformation.first, fileIn);
		SerializeHelper::SerializeSequenceContainer(assetNameToTransformation.second, fileIn);
	}
}

void MapAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	m_iblMaterialName = DeserializeHelper::DeserializeString(fileIn);
	const size_t assetNameToTransformationsCount = DeserializeHelper::DeserializeContainerSize(fileIn);

	for (size_t idx = 0; idx < assetNameToTransformationsCount; ++idx)
	{
		string assetName = DeserializeHelper::DeserializeString(fileIn);
		vector<XMMATRIX> transformations = DeserializeHelper::DeserializeSequenceContainer<vector<XMMATRIX>>(fileIn);
		m_assetNameToTransformations.emplace(move(assetName), move(transformations));
	}
}
