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

void MeshAsset::Serialize(FILE* fileIn)
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeVectorHelper(m_positions, fileIn);
	SerializeHelper::SerializeVectorHelper(m_uvTextures, fileIn);
	SerializeHelper::SerializeVectorHelper(m_normals, fileIn);
	SerializeHelper::SerializeVectorHelper(m_indices, fileIn);

	SerializeHelper::SerializeString(m_defaultMaterialName, fileIn);
}

void MeshAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	m_positions = SerializeHelper::DeserializeVectorHelper<XMFLOAT3>(fileIn);
	m_uvTextures = SerializeHelper::DeserializeVectorHelper<XMFLOAT2>(fileIn);
	m_normals = SerializeHelper::DeserializeVectorHelper<XMFLOAT3>(fileIn);
	m_indices = SerializeHelper::DeserializeVectorHelper<uint32_t>(fileIn);

	m_defaultMaterialName = SerializeHelper::DeserializeString(fileIn);
}
