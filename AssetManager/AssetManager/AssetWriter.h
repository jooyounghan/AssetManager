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
	virtual std::unordered_map<EAssetType, std::vector<AAsset*>> SaveAsAssets(const std::string& filePath) = 0;
	virtual bool IsAcceptableFilePath(const std::string& filePath) const = 0;

protected:
	void SaveAssets(const EAssetType& assetType, const std::vector<AAsset*>& assets) const;
};

