#include "pch.h"
#include "MaterialAsset.h"

using namespace std;
using namespace DirectX;

MaterialAsset::MaterialAsset(
	const string& assetPathIn, 
	const string& assetNameIn
)
	: AAsset(assetPathIn, assetNameIn), m_f0(), m_heightScale(0.f)
{
}

MaterialAsset::~MaterialAsset() {}

void MaterialAsset::SetMaterialTexture(
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

void MaterialAsset::UpdateBaseTextureAsset(EMaterialTexture materialTextureType, IBaseTextureProvider& provider)
{
	const size_t& materialTextureIdx = static_cast<size_t>(materialTextureType);
	m_materialTexture[materialTextureIdx] = provider.GetBaseTextureAsset(m_materialTextureName[materialTextureIdx]);
}

void MaterialAsset::SetF0(const float& x, const float& y, const float& z)
{
	m_f0.x = x;
	m_f0.y = y;
	m_f0.z = z;
	m_isModified = true;
}

void MaterialAsset::SetHeightScale(const float& heightScale)
{
	m_heightScale = heightScale;
	m_isModified = true;
}

void MaterialAsset::Serialize(FILE* fileIn)
{
	for (size_t materialIdx = 0; materialIdx < MaterialTextureCount; ++materialIdx)
	{
		SerializeHelper(m_materialTexture[materialIdx], fileIn);
	}
	fwrite(&m_f0, sizeof(XMFLOAT3), 1, fileIn);
	fwrite(&m_heightScale, sizeof(float), 1, fileIn);
}

void MaterialAsset::Deserialize(FILE* fileIn)
{
	for (size_t materialIdx = 0; materialIdx < MaterialTextureCount; ++materialIdx)
	{
		string materialAssetName = SerializeHelper::DeserializeString(fileIn);
		m_materialTextureName[materialIdx] = materialAssetName;
	}
	fread(&m_f0, sizeof(XMFLOAT3), 1, fileIn);
	fread(&m_heightScale, sizeof(float), 1, fileIn);
}

void MaterialAsset::SerializeHelper(
	const shared_ptr<BaseTextureAsset>& materialTexture,
	FILE* fileIn 
)
{
	SerializeHelper::SerializeString(
		materialTexture != nullptr ? materialTexture->GetAssetName() : "", 
		fileIn
	);
}