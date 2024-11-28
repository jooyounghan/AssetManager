#include "pch.h"
#include "MeshAsset.h"

using namespace std;
using namespace DirectX;


void MeshPartsData::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeSequenceContainer(m_positions, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_uvTextures, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_normals, fileIn);

	SerializeHelper::SerializeContainerSize(m_offsetToIndices, fileIn);
	for (auto& offsetToIndice : m_offsetToIndices)
	{
		SerializeHelper::SerializeElement(offsetToIndice.first, fileIn);
		SerializeHelper::SerializeSequenceContainer(offsetToIndice.second, fileIn);
	}

}

void MeshPartsData::Deserialize(FILE* fileIn)
{
	m_positions = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT3>>(fileIn);
	m_uvTextures = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT2>>(fileIn);
	m_normals = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT3>>(fileIn);

	const size_t containerCount = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerCount; ++idx)
	{
		const uint32_t offset = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
		const vector<uint32_t> indices = DeserializeHelper::DeserializeSequenceContainer<vector<uint32_t>>(fileIn);
		m_offsetToIndices.emplace(offset, indices);
	}
}

AMeshAsset::AMeshAsset(const string& assetName)
	: AAsset(assetName)
{
}

AMeshAsset::~AMeshAsset()
{
}

void AMeshAsset::UpdateDefaultMaterialAsset(IModelMaterialProvider& provider)
{
	m_defaultMaterials.clear();
	for (const string& defaultMaterialName : m_defaultMaterialNames)
	{
		m_defaultMaterials.emplace_back(provider.GetModelMaterialAsset(defaultMaterialName));
	}
}

void AMeshAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_defaultMaterialNames, fileIn);
	for (const string& defaultMaterialName : m_defaultMaterialNames)
	{
		SerializeHelper::SerializeString(defaultMaterialName, fileIn);
	}
}

void AMeshAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	const size_t containerCount = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerCount; ++idx)
	{
		m_defaultMaterialNames.emplace_back(DeserializeHelper::DeserializeString(fileIn));
	}
}
