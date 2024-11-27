#include "pch.h"
#include "IBLMaterialAsset.h"

using namespace std;
using namespace DirectX;

IBLMaterialAsset::IBLMaterialAsset(const string& assetName)
	: AAsset(assetName), m_exposure(1.f), m_gamma(1.f)
{
}

IBLMaterialAsset::~IBLMaterialAsset()
{
}

void IBLMaterialAsset::UpdateIBLBaseTextureAsset(
	EIBLMaterialTexture iblMaterialTextureType, 
	IScratchTextureProvider& provider
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(iblMaterialTextureType);
	m_materialTexture[materialTextureIdx] = provider.GetScratchTextureAsset(m_materialTextureName[materialTextureIdx]);
}

void IBLMaterialAsset::SetIBLMaterialTexture(
	EIBLMaterialTexture iblMaterialTextureType, 
	const string& iblmaterialTextureIn, 
	IScratchTextureProvider& provider
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(iblMaterialTextureType);
	m_materialTextureName[materialTextureIdx] = iblmaterialTextureIn;
	UpdateIBLBaseTextureAsset(iblMaterialTextureType, provider);
	m_isModified = true;
}

void IBLMaterialAsset::SetToneMappingProperties(const float& exposure, const float& gamma)
{
	m_exposure = exposure;
	m_gamma = gamma;
	m_isModified = true;
}

void IBLMaterialAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < IBLMaterialTextureCount; ++materialIdx)
	{
		SerializeAssetName(m_materialTexture[materialIdx], fileIn);
	}
	fwrite(&m_exposure, sizeof(float), 1, fileIn);
	fwrite(&m_gamma, sizeof(float), 1, fileIn);
}

void IBLMaterialAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < IBLMaterialTextureCount; ++materialIdx)
	{
		string materialAssetName = DeserializeHelper::DeserializeString(fileIn);
		m_materialTextureName[materialIdx] = materialAssetName;
	}
	fread(&m_exposure, sizeof(float), 1, fileIn);
	fread(&m_gamma, sizeof(float), 1, fileIn);

}

