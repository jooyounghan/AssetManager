#pragma once
#include "BaseTextureAsset.h"
#include "DirectXMath/DirectXMath.h"

enum class EMaterialTexture
{
	MATERIAL_TEXTURE_AMBIENTOCCULUSION,
	MATERIAL_TEXTURE_SPECULAR,
	MATERIAL_TEXTURE_DIFFUSE,
	MATERIAL_TEXTURE_ROUGHNESS,
	MATERIAL_TEXTURE_METALIC,
	MATERIAL_TEXTURE_NORMAL,
	MATERIAL_TEXTURE_HEIGHT,
	MATERIAL_TEXTURE_EMISSIVE,
	MATERIAL_TEXTURE_COUNT
};

constexpr size_t MaterialTextureCount = static_cast<size_t>(EMaterialTexture::MATERIAL_TEXTURE_COUNT);

class MaterialAsset : public AAsset
{
public:
	MaterialAsset(
		const std::string& assetPathIn,
		const std::string& assetNameIn
	);
	virtual ~MaterialAsset();

protected:
	std::string m_materialTextureName[MaterialTextureCount];
	std::shared_ptr<BaseTextureAsset> m_materialTexture[MaterialTextureCount];

protected:
	 DirectX::XMFLOAT3 m_f0;
	float m_heightScale;

public:
	void SetMaterialTexture(
		EMaterialTexture materialTextureType, 
		const std::string& materialTextureIn, 
		IBaseTextureProvider& provider
	);
	void UpdateBaseTextureAsset(
		EMaterialTexture materialTextureType, 
		IBaseTextureProvider& provider
	);

public:
	void SetF0(const float& x, const float& y, const float& z);
	void SetHeightScale(const float& heightScale);

public:
	virtual void Serialize(FILE* fileIn) override;
	virtual void Deserialize(FILE* fileIn) override;


private:
	void SerializeHelper(
		const std::shared_ptr<BaseTextureAsset>& materialTexture,
		FILE* fileIn
	);
};

class IMaterialProvider
{
public:
	virtual std::shared_ptr<MaterialAsset> GetMaterialAsset(
		const std::string& textureName
	) = 0;
};

