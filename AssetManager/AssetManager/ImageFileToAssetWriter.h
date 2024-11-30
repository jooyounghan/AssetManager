#pragma once
#include "AssetWriter.h"

class ImageFileToAssetWriter : public AAssetWriter
{
public:
	ImageFileToAssetWriter(const std::string& assetSavePath);
	virtual ~ImageFileToAssetWriter();

protected:
	static std::string ExrExtension;
	static std::string HdrExtension;
	static std::string PngExtension;
	static std::string JpegExtension;

public:
	static std::vector<std::string> ImageFileExtensions;

public:
	virtual std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> SaveAsAssets(const std::string& filePath) override;
	virtual bool IsAcceptableFilePath(const std::string& filePath) override;

private:
	std::shared_ptr<AAsset> LoadBaseTexureAsset(const std::string& filePath, const std::string& fileName);
	std::shared_ptr<AAsset> LoadScratchTexureByEXRAsset(const std::string& filePath, const std::string& fileName);
	std::shared_ptr<AAsset> LoadScratchTexureByHDRAsset(const std::string& filePath, const std::string& fileName);
};

