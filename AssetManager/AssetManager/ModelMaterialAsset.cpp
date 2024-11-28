#include "pch.h"
#include "ModelMaterialAsset.h"

using namespace std;
using namespace DirectX;

ModelMaterialAsset::ModelMaterialAsset(const string& assetName)
	: AAsset(assetName), m_f0(), m_heightScale(0.f)
{
}

ModelMaterialAsset::~ModelMaterialAsset() {}

void ModelMaterialAsset::UpdateModelBaseTextureAsset(
	EModelMaterialTexture modelMaterialTextureType,
	IBaseTextureProvider& provider
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(modelMaterialTextureType);
	m_materialTexture[materialTextureIdx] = provider.GetBaseTextureAsset(m_materialTextureName[materialTextureIdx]);
}

void ModelMaterialAsset::SetModelMaterialTexture(
	EModelMaterialTexture modelMaterialTextureType,
	const string& modelMaterialTextureIn,
	IBaseTextureProvider& provider
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(modelMaterialTextureType);
	m_materialTextureName[materialTextureIdx] = modelMaterialTextureIn;
	UpdateModelBaseTextureAsset(modelMaterialTextureType, provider);
	m_isModified = true;
}

void ModelMaterialAsset::SetModelMaterialTexture(
	EModelMaterialTexture modelMaterialTextureType, 
	shared_ptr<BaseTextureAsset> baseTextureAsset
)
{
	const size_t& materialTextureIdx = static_cast<size_t>(modelMaterialTextureType);
	m_materialTextureName[materialTextureIdx] = baseTextureAsset->GetAssetName();
	m_materialTexture[materialTextureIdx] = baseTextureAsset;
}

void ModelMaterialAsset::SetModelMaterialProperties(
	const DirectX::XMFLOAT3& f0, 
	const float& heightScale
)
{
	m_f0 = f0;
	m_heightScale = heightScale;
	m_isModified = true;
}


void ModelMaterialAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < ModelMaterialTextureCount; ++materialIdx)
	{
		SerializeAssetName(m_materialTexture[materialIdx], fileIn);
	}
	fwrite(&m_f0, sizeof(XMFLOAT3), 1, fileIn);
	fwrite(&m_heightScale, sizeof(float), 1, fileIn);
}

void ModelMaterialAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);
	for (size_t materialIdx = 0; materialIdx < ModelMaterialTextureCount; ++materialIdx)
	{
		string materialAssetName = DeserializeHelper::DeserializeString(fileIn);
		m_materialTextureName[materialIdx] = materialAssetName;
	}
	fread(&m_f0, sizeof(XMFLOAT3), 1, fileIn);
	fread(&m_heightScale, sizeof(float), 1, fileIn);
}

