#pragma once
#include "AssetWriter.h"

struct aiScene;

class ModelFileToAssetWriter : AAssetWriter
{
public:
	ModelFileToAssetWriter(const std::string& assetSavePath);
	virtual ~ModelFileToAssetWriter();

public:
	static std::vector<std::string> ModelFileExtensions;

public:
	virtual std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> WriteToAssets(const std::string& filePath) override;

private:
	std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> LoadTexturesAndMaterials(const aiScene* scene);
};

