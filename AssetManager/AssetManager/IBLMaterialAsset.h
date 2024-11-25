#pragma once
#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"

enum class EIBLMaterialTexture
{
	IBL_MATERIAL_TEXTURE_BACKGROUND,
	IBL_MATERIAL_TEXTURE_SPECULAR,
	IBL_MATERIAL_TEXTURE_DIFFUSE,
	IBL_MATERIAL_TEXTURE_BRDF,
	IBL_MATERIAL_TEXTURE_COUNT
};

constexpr size_t IBLMaterialTextureCount = static_cast<size_t>(EIBLMaterialTexture::IBL_MATERIAL_TEXTURE_COUNT);

class IBLMaterialAsset : public AAsset
{
public:
	IBLMaterialAsset(const std::string& assetNameIn);
	virtual ~IBLMaterialAsset();

protected:
	std::string m_materialTextureName[IBLMaterialTextureCount];
	std::shared_ptr<ScratchTextureAsset> m_materialTexture[IBLMaterialTextureCount];

public:
	void UpdateIBLBaseTextureAsset(
		EIBLMaterialTexture iblMaterialTextureType,
		IScratchTextureProvider& provider
	);
	void SetIBLMaterialTexture(
		EIBLMaterialTexture iblMaterialTextureType,
		const std::string& iblmaterialTextureIn,
		IScratchTextureProvider& provider
	);

protected:
	float m_exposure;
	float m_gamma;

public:
	MakeGetter(m_exposure, Exposure);
	MakeGetter(m_gamma, Gamma);

public:
	void SetToneMappingProperties(const float& exposure, const float& gamma);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class IIBLMaterialProvider
{
public:
	virtual std::shared_ptr<IBLMaterialAsset> GetIBLMaterialAsset(
		const std::string& textureName
	) = 0;
};
