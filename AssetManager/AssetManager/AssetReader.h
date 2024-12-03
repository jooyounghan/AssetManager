#pragma once

#include "Asset.h"
#include "AssetWrapper.h"
#include "TopologySorter.h"

struct SAssetPreloadArgs
{
	std::string assetPath;
	long lastReadPoint = NULL;
};

class AssetReader
{
public:
	AssetReader(const std::string& assetBasePath);
	~AssetReader();

protected:
	std::string m_assetBasePath;

protected:
	TopologySorter<EAssetType> m_topologySorter;
	std::unordered_map<EAssetType, std::vector<SAssetPreloadArgs>> m_assetTypeToPreloadArgs;

public:
	void UpdatePreloadArgs();

public:
	std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> GetLoadedAsset() const;
};

