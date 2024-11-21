#pragma once
#include "ATextureAsset.h"

class BaseTextureAsset : public ATextureAsset
{
public:
	BaseTextureAsset(const std::string& assetPathIn, const std::string& assetNameIn);
	BaseTextureAsset(
		const std::string& assetPathIn, 
		const std::string& assetNameIn,
		const unsigned int& widthIn,
		const unsigned int& heightIn,
		uint8_t* imageBufferIn
	);

	virtual ~BaseTextureAsset();

public:
	virtual std::vector<std::vector<uint8_t>> CompressDataArray(const std::vector<std::vector<uint8_t>>& originalBufferPerArray) override;
	virtual std::vector<std::vector<uint8_t>> DecompressDataArray() override;
};

class IBaseTextureProvider
{
public:
	virtual std::shared_ptr<BaseTextureAsset> GetBaseTextureAsset(
		const std::string& textureName
	) = 0;
};

