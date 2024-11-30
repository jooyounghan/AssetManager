#pragma once
#include "AssetReader.h"
#include "AssetWriter.h"
#include "ResourceManager.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

protected:
	std::vector<AssetReader> m_assetReaders;
	std::vector<std::shared_ptr<AAssetWriter>> m_assetWriters;
	ResourceManager m_resourceManager;

public:
	void RegisterAssetReadPath(const std::string& readPath);
	void RegisterAssetWritePath(const std::string& writePath);

public:
	void PreloadAsset();
	void WrtieFileAsAsset(const std::string filePath);
};

