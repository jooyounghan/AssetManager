#pragma once
#include "ATextureAsset.h"

class BaseTextureAsset : public ATextureAsset
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

public:
	virtual std::vector<UINT> GetRowPitchArray() override;
};

class IBaseTextureProvider
{
public:
	virtual std::shared_ptr<BaseTextureAsset> GetBaseTextureAsset(
		const std::string& assetName
	) = 0;
};

