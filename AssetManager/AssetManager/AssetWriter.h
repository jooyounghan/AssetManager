#pragma once
#include "AssetWrapper.h"
#include <memory>
#include <unordered_map>

class AAssetWriter
{
public:
	AAssetWriter(const std::string& assetSavePath);
	~AAssetWriter();

protected:
	std::string m_assetSavePath;

public:
	virtual std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> WriteToAssets(const std::string& filePath) = 0;
};

