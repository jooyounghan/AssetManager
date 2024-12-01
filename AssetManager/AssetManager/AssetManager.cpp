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
	const vector<shared_ptr<BaseTextureAsset>> bitmapResourceAssets = m_resourceManager.LoadBitmapResources();
	for (const shared_ptr<BaseTextureAsset>& bitmapResourceAsset : bitmapResourceAssets)
	{
		m_assetNameToAssets[EAssetType::ASSET_TYPE_RESOURCE].emplace(bitmapResourceAsset->GetAssetName(), bitmapResourceAsset);
	}

	for (AssetReader& assetReader : m_assetReaders)
	{
		assetReader.UpdatePreloadArgs();
		unordered_map<EAssetType, vector<shared_ptr<AAsset>>> assetTypesToLoadedAssets = assetReader.GetLoadedAsset();

		for (const auto& assetTypeToLoadedAssets : assetTypesToLoadedAssets)
		{
			const EAssetType& assetType = assetTypeToLoadedAssets.first;
			const vector<shared_ptr<AAsset>> loadedAssets = assetTypeToLoadedAssets.second;
			for (const shared_ptr<AAsset>& loadedAsset : loadedAssets)
			{
				m_assetNameToAssets[assetType].emplace(loadedAsset->GetAssetName(), loadedAsset);
			}
		}
	}
}

void AssetManager::WrtieFileAsAsset(const std::string filePath)
{
	for (shared_ptr<AAssetWriter>& assetWriter : m_assetWriters)
	{
		if (assetWriter->IsAcceptableFilePath(filePath))
		{
			const unordered_map<EAssetType, vector<shared_ptr<AAsset>>> savedTypesToAssets = assetWriter->SaveAsAssets(filePath);

			for (const auto& savedTypeToAssets : savedTypesToAssets)
			{
				const EAssetType& assetType = savedTypeToAssets.first;
				const vector<shared_ptr<AAsset>>& assets = savedTypeToAssets.second;
				for (const shared_ptr<AAsset>& asset : assets)
				{
					m_assetNameToAssets[assetType].emplace(asset->GetAssetName(), asset);
				}
			}

		}
	}
}

shared_ptr<BaseTextureAsset> AssetManager::GetBaseTextureAsset(const string& assetName)
{
	return GetAssetHelper<BaseTextureAsset>(EAssetType::ASSET_TYPE_BASE_TEXTURE, assetName);
}

shared_ptr<ScratchTextureAsset> AssetManager::GetScratchTextureAsset(const string& assetName)
{
	return GetAssetHelper<ScratchTextureAsset>(EAssetType::ASSET_TYPE_SCRATCH_TEXTURE, assetName);
}

shared_ptr<StaticMeshAsset> AssetManager::GetStaticMeshAsset(const string& assetName)
{
	return GetAssetHelper<StaticMeshAsset>(EAssetType::ASSET_TYPE_STATIC, assetName);
}

shared_ptr<SkeletalMeshAsset> AssetManager::GetSkeletalMeshAsset(const string& assetName)
{
	return GetAssetHelper<SkeletalMeshAsset>(EAssetType::ASSET_TYPE_SKELETAL, assetName);
}

shared_ptr<ModelMaterialAsset> AssetManager::GetModelMaterialAsset(const string& assetName)
{
	return GetAssetHelper<ModelMaterialAsset>(EAssetType::ASSET_TYPE_MODEL_MATERIAL, assetName);
}

shared_ptr<IBLMaterialAsset> AssetManager::GetIBLMaterialAsset(const string& assetName)
{
	return GetAssetHelper<IBLMaterialAsset>(EAssetType::ASSET_TYPE_IBL_MATERIAL, assetName);
}

shared_ptr<BoneAsset> AssetManager::GetBoneAsset(const string& assetName)
{
	return GetAssetHelper<BoneAsset>(EAssetType::ASSET_TYPE_BONE, assetName);
}

shared_ptr<AnimationAsset> AssetManager::GetAnimationAsset(const string& assetName)
{
	return GetAssetHelper<AnimationAsset>(EAssetType::ASSET_TYPE_ANIMATION, assetName);
}

shared_ptr<MapAsset> AssetManager::GetMapAsset(const string& assetName)
{
	return GetAssetHelper<MapAsset>(EAssetType::ASSET_TYPE_MAP, assetName);
}

shared_ptr<BaseTextureAsset> AssetManager::GetResourceAsset(const string& assetName)
{
	return shared_ptr<BaseTextureAsset>();
}


template<typename T>
inline shared_ptr<T> AssetManager::GetAssetHelper(const EAssetType& asssetType, const string& assetName)
{
	const unordered_map<string, shared_ptr<AAsset>>& assetNameToAssets = m_assetNameToAssets[asssetType];
	if (assetNameToAssets.find(assetName) != assetNameToAssets.end())
	{
		return dynamic_pointer_cast<T>(assetNameToAssets.at(assetName));
	}
	return nullptr;
}
