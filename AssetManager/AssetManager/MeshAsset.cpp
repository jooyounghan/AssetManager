#include "pch.h"
#include "MeshAsset.h"

using namespace std;
using namespace DirectX;


const vector<uint32_t>& MeshPartsData::GetIndices(const uint32_t& offset)
{
	if (m_offsetToIndices.find(offset) != m_offsetToIndices.end())
	{
		return m_offsetToIndices[offset];
	}
	return m_emptyIndices;
}

const DirectX::XMFLOAT3& MeshPartsData::GetPosition(const size_t& index)
{
	if (m_positions.size() > index)
	{
		return m_positions[index];
	}
	return m_emptyFloat3;
}

const DirectX::XMFLOAT2& MeshPartsData::GetUVTextureCoord(const size_t& index)
{
	if (m_uvTextures.size() > index)
	{
		return m_uvTextures[index];
	}
	return m_emptyFloat2;
}

const DirectX::XMFLOAT3& MeshPartsData::GetNormal(const size_t& index)
{
	if (m_normals.size() > index)
	{
		return m_normals[index];
	}
	return m_emptyFloat3;
}

void MeshPartsData::AddPosition(const float& x, const float& y, const float& z)
{
	m_positions.emplace_back(XMFLOAT3(x, y, z));
}

void MeshPartsData::AddUVTexture(const float& u, const float& v)
{
	m_uvTextures.emplace_back(XMFLOAT2(u, v));
}

void MeshPartsData::AddNormal(const float& x, const float& y, const float& z)
{
	m_normals.emplace_back(XMFLOAT3(x, y, z));
}

void MeshPartsData::AddIndex(const uint32_t& offset, const uint32_t index)
{
	m_offsetToIndices[offset].emplace_back(index);
}

uint32_t MeshPartsData::GetVerticesCount()
{
	return static_cast<uint32_t>(m_positions.size());
}

vector<uint32_t> MeshPartsData::GetVertexOffsets()
{
	vector<uint32_t> result;
	for (const auto& offsetToIndice : m_offsetToIndices)
	{
		result.emplace_back(offsetToIndice.first);
	}
	return result;
}

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
