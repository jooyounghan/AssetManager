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
	ScratchTextureAsset() = default;
	ScratchTextureAsset(
		const std::string& assetName,
		const DirectX::ScratchImage& scratch,
		const DirectX::TexMetadata& metaData
	);
	virtual ~ScratchTextureAsset();

protected:
	std::vector<UINT> m_rowPitchPerArray;

public:
	virtual std::vector<UINT> GetRowPitchArray() override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class IScratchTextureProvider
{
public:
	virtual std::shared_ptr<ScratchTextureAsset> GetScratchTextureAsset(
		const std::string& assetName
	) = 0;
};
