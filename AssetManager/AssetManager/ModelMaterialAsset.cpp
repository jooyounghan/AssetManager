#include "pch.h"
#include "ModelMaterialAsset.h"

using namespace std;
using namespace DirectX;

ModelMaterialAsset::ModelMaterialAsset(const string& assetNameIn)
	: AAsset(assetNameIn), m_f0(), m_heightScale(0.f)
{
}

ModelMaterialAsset::~ModelMaterialAsset() {}

void ModelMaterialAsset::SetMaterialTexture(
	EMaterialTexture materialTextureType,
	const string& materialTextureIn,
	IBaseTextureProvider& provider
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(materialTextureType);
	m_materialTextureName[materialTextureIdx] = materialTextureIn;
	m_materialTexture[materialTextureIdx] = provider.GetBaseTextureAsset(materialTextureIn);
	m_isModified = true;
}

void ModelMaterialAsset::UpdateBaseTextureAsset(EMaterialTexture materialTextureType, IBaseTextureProvider& provider)
{
	const size_t& materialTextureIdx = static_cast<size_t>(materialTextureType);
	m_materialTexture[materialTextureIdx] = provider.GetBaseTextureAsset(m_materialTextureName[materialTextureIdx]);
}

void ModelMaterialAsset::SetF0(const float& x, const float& y, const float& z)
{
	m_f0.x = x;
	m_f0.y = y;
	m_f0.z = z;
	m_isModified = true;
}

void ModelMaterialAsset::SetHeightScale(const float& heightScale)
{
	m_heightScale = heightScale;
	m_isModified = true;
}

void ModelMaterialAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < MaterialTextureCount; ++materialIdx)
	{
		SerializeHelper(m_materialTexture[materialIdx], fileIn);
	}
	fwrite(&m_f0, sizeof(XMFLOAT3), 1, fileIn);
	fwrite(&m_heightScale, sizeof(float), 1, fileIn);
}

void ModelMaterialAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < MaterialTextureCount; ++materialIdx)
	{
		string materialAssetName = DeserializeHelper::DeserializeString(fileIn);
		m_materialTextureName[materialIdx] = materialAssetName;
	}
	fread(&m_f0, sizeof(XMFLOAT3), 1, fileIn);
	fread(&m_heightScale, sizeof(float), 1, fileIn);
}

void ModelMaterialAsset::SerializeHelper(
	const shared_ptr<BaseTextureAsset>& materialTexture,
	FILE* fileIn 
) const
{
	SerializeHelper::SerializeString(
		materialTexture != nullptr ? materialTexture->GetAssetName() : "", 
		fileIn
	);
}