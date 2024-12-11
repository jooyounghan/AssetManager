#pragma once
#include "AssetReader.h"
#include "AssetWriter.h"
#include "ResourceManager.h"

#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "ModelMaterialAsset.h"
#include "IBLMaterialAsset.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"
#include "MapAsset.h"

class AssetManager : public IBaseTextureProvider, public IScratchTextureProvider, public IStaticMeshProvider,
	public ISkeletalMeshProvider, public IModelMaterialProvider, public IIBLMaterialProvider, public IBoneProvider,
	public IAnimationProvider, public IMapProvider, public IResourceProvider
{
public:
	AssetManager();
	~AssetManager();

protected:
	std::vector<AssetReader> m_assetReaders;
	std::vector<AAssetWriter*> m_assetWriters;
	ResourceManager m_resourceManager;

protected:
	std::unordered_map<EAssetType, std::unordered_map<std::string, AAsset*>> m_assetNameToAssets;
	
public:
	void RegisterAssetReadPath(const std::string& readPath);
	void RegisterAssetWritePath(const std::string& writePath);

public:
	void PreloadAsset();
	void WrtieFileAsAsset(const std::string filePath);

public:
	virtual BaseTextureAsset* const GetBaseTextureAsset(const std::string& assetName) override;
	virtual ScratchTextureAsset* const GetScratchTextureAsset(const std::string& assetName) override;
	virtual StaticMeshAsset* const GetStaticMeshAsset(const std::string& assetName) override;
	virtual SkeletalMeshAsset* const GetSkeletalMeshAsset(const std::string& assetName) override;
	virtual ModelMaterialAsset* const GetModelMaterialAsset(const std::string& assetName) override;
	virtual IBLMaterialAsset* const GetIBLMaterialAsset(const std::string& assetName) override;
	virtual BoneAsset* const GetBoneAsset(const std::string& assetName) override;
	virtual AnimationAsset* const GetAnimationAsset(const std::string& assetName) override;
	virtual MapAsset* const GetMapAsset(const std::string& assetName) override;
	virtual BaseTextureAsset* const GetResourceAsset(const std::string& assetName) override;

private:
	template<typename T>
	T* const GetAssetHelper(const EAssetType& asssetType, const std::string& assetName);
};
