#include "pch.h"
#include "MeshAsset.h"

using namespace std;
using namespace DirectX;

MeshAsset::MeshAsset(const string& assetPathIn, const string& assetNameIn)
	: AAsset(assetPathIn, assetNameIn)
{
}

MeshAsset::~MeshAsset()
{
}

void MeshAsset::UpdateDefaultMaterialAsset(IMaterialProvider& provider)
{
	m_defaultMaterial = provider.GetMaterialAsset(m_defaultMaterialName);
}

void MeshAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeSequenceContainer(m_positions, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_uvTextures, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_normals, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_indices, fileIn);

	SerializeHelper::SerializeString(m_defaultMaterialName, fileIn);
}

void MeshAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	m_positions = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT3>>(fileIn);
	m_uvTextures = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT2>>(fileIn);
	m_normals = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT3>>(fileIn);
	m_indices = DeserializeHelper::DeserializeSequenceContainer<vector<uint32_t>>(fileIn);

	m_defaultMaterialName = DeserializeHelper::DeserializeString(fileIn);
}
