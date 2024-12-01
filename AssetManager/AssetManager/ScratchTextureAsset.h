#pragma once
#include "TextureAsset.h"

namespace DirectX 
{
	class ScratchImage;
	struct TexMetadata;
}

class ScratchTextureAsset : public TextureAsset
{
public:
	ScratchTextureAsset() = default;
	ScratchTextureAsset(
		const std::string& assetName,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);
	virtual ~ScratchTextureAsset();
};

class IScratchTextureProvider
{
public:
	virtual std::shared_ptr<ScratchTextureAsset> GetScratchTextureAsset(
		const std::string& assetName
	) = 0;
};
