#include "pch.h"
#include "AssetManager.h"

#include "ModelFileToAssetWriter.h"
#include "ImageFileToAssetWriter.h"

using namespace std;

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::RegisterAssetReadPath(const std::string& readPath)
{
	m_assetReaders.emplace_back(readPath);
}

void AssetManager::RegisterAssetWritePath(const std::string& writePath)
{
	m_assetWriters.emplace_back(make_shared<ModelFileToAssetWriter>(writePath));
	m_assetWriters.emplace_back(make_shared<ImageFileToAssetWriter>(writePath));
}

void AssetManager::PreloadAsset()
{
	auto test2 = m_resourceManager.LoadBitmapResources();



	for (AssetReader& assetReader : m_assetReaders)
	{
		assetReader.UpdatePreloadArgs();
		unordered_map<EAssetType, vector<shared_ptr<AAsset>>> loadedAssets = assetReader.GetLoadedAsset();

	}
}

void AssetManager::WrtieFileAsAsset(const std::string filePath)
{
	for (shared_ptr<AAssetWriter>& assetWriter : m_assetWriters)
	{
		if (assetWriter->IsAcceptableFilePath(filePath))
		{
			assetWriter->SaveAsAssets(filePath);
		}
	}
}
