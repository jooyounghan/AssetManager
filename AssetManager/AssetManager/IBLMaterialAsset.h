#pragma once
#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"

class IBLMaterialAsset : public AAsset
{
public:
	IBLMaterialAsset(const std::string& assetNameIn);
	virtual ~IBLMaterialAsset();

protected:
	std::string m_backgroundTextureName;
	std::string m_specularTextureName;
	std::string m_diffuseTextureName;
	std::string m_brdfTextureName;

protected:
	std::shared_ptr<ScratchTextureAsset> m_backgroundTexture;
	std::shared_ptr<ScratchTextureAsset> m_specularTexture;
	std::shared_ptr<ScratchTextureAsset> m_diffuseTexture;
	std::shared_ptr<ScratchTextureAsset> m_brdfTexture;

	/*EXR 1 / DDS 3*/
protected:
	float m_exposure;
	float m_gamma;

};

