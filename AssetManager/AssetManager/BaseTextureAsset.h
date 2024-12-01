#pragma once
#include "TextureAsset.h"

class BaseTextureAsset : public TextureAsset
{
public:
	BaseTextureAsset() = default;
	BaseTextureAsset(
		const std::string& assetName,
		const unsigned int& widthIn,
		const unsigned int& heightIn,
		uint8_t* imageBufferIn
	);

	virtual ~BaseTextureAsset();
};

class IBaseTextureProvider
{
public:
	virtual std::shared_ptr<BaseTextureAsset> GetBaseTextureAsset(
		const std::string& assetName
	) = 0;
};

