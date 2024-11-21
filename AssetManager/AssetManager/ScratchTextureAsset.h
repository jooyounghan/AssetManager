#pragma once
#include "ATextureAsset.h"

namespace DirectX 
{
	class ScratchImage;
	struct TexMetadata;
}

class ScratchTextureAsset : public ATextureAsset
{
public:
	ScratchTextureAsset(
		const std::string& assetPathIn,
		const std::string& assetNameIn
	);
	ScratchTextureAsset(
		const std::string& assetPathIn,
		const std::string& assetNameIn,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);
	virtual ~ScratchTextureAsset();

public:
	virtual std::vector<std::vector<uint8_t>> CompressDataArray(
		const std::vector<std::vector<uint8_t>>& originalBufferPerArray
	) override;
	virtual std::vector<std::vector<uint8_t>> DecompressDataArray() override;

};

