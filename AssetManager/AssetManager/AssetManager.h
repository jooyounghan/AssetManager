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
	std::vector<std::shared_ptr<AAssetWriter>> m_assetWriters;
	ResourceManager m_resourceManager;

protected:
	std::unordered_map<EAssetType, std::unordered_map<std::string, std::shared_ptr<AAsset>>> m_assetNameToAssets;
	
public:
	void RegisterAssetReadPath(const std::string& readPath);
	void RegisterAssetWritePath(const std::string& writePath);

public:
	void PreloadAsset();
	void WrtieFileAsAsset(const std::string filePath);

public:
	virtual std::shared_ptr<BaseTextureAsset> GetBaseTextureAsset(const std::string& assetName) override;
	virtual std::shared_ptr<ScratchTextureAsset> GetScratchTextureAsset(const std::string& assetName) override;
	virtual std::shared_ptr<StaticMeshAsset> GetStaticMeshAsset(const std::string& assetName) override;
	virtual std::shared_ptr<SkeletalMeshAsset> GetSkeletalMeshAsset(const std::string& assetName) override;
	virtual std::shared_ptr<ModelMaterialAsset> GetModelMaterialAsset(const std::string& assetName) override;
	virtual std::shared_ptr<IBLMaterialAsset> GetIBLMaterialAsset(const std::string& assetName) override;
	virtual std::shared_ptr<BoneAsset> GetBoneAsset(const std::string& assetName) override;
	virtual std::shared_ptr<AnimationAsset> GetAnimationAsset(const std::string& assetName) override;
	virtual std::shared_ptr<MapAsset> GetMapAsset(const std::string& assetName) override;
	virtual std::shared_ptr<BaseTextureAsset> GetResourceAsset(const std::string& assetName) override;

private:
	template<typename T>
	std::shared_ptr<T> GetAssetHelper(const EAssetType& asssetType, const std::string& assetName);
};
