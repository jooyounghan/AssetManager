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
	for (AAssetWriter* assetWriter : m_assetWriters)
	{
		delete assetWriter;
	}

	for (auto& assetNameToAssets : m_assetNameToAssets)
	{
		for (auto& assetNameToAsset : assetNameToAssets.second)
		{
			delete assetNameToAsset.second;
		}
	}
	;
}

void AssetManager::RegisterAssetReadPath(const std::string& readPath)
{
	m_assetReaders.emplace_back(readPath);
}

void AssetManager::RegisterAssetWritePath(const std::string& writePath)
{
	m_assetWriters.emplace_back(new ModelFileToAssetWriter(writePath));
	m_assetWriters.emplace_back(new ImageFileToAssetWriter(writePath));
}

void AssetManager::PreloadAsset()
{
	const vector<BaseTextureAsset*> bitmapResourceAssets = m_resourceManager.LoadBitmapResources();
	for (BaseTextureAsset* const bitmapResourceAsset : bitmapResourceAssets)
	{
		m_assetNameToAssets[EAssetType::ASSET_TYPE_RESOURCE].emplace(bitmapResourceAsset->GetAssetName(), bitmapResourceAsset);
	}

	for (AssetReader& assetReader : m_assetReaders)
	{
		assetReader.UpdatePreloadArgs();
		unordered_map<EAssetType, vector<AAsset*>> assetTypesToLoadedAssets = assetReader.GetLoadedAsset();

		for (const auto& assetTypeToLoadedAssets : assetTypesToLoadedAssets)
		{
			const EAssetType& assetType = assetTypeToLoadedAssets.first;
			const vector<AAsset*> loadedAssets = assetTypeToLoadedAssets.second;
			for (AAsset* const loadedAsset : loadedAssets)
			{
				m_assetNameToAssets[assetType][loadedAsset->GetAssetName()] = loadedAsset;
			}
		}
	}
}

void AssetManager::WrtieFileAsAsset(const std::string filePath)
{
	for (AAssetWriter* const assetWriter : m_assetWriters)
	{
		if (assetWriter->IsAcceptableFilePath(filePath))
		{
			const unordered_map<EAssetType, vector<AAsset*>> savedTypesToAssets = assetWriter->SaveAsAssets(filePath);

			for (const auto& savedTypeToAssets : savedTypesToAssets)
			{
				const EAssetType& assetType = savedTypeToAssets.first;
				const vector<AAsset*>& assets = savedTypeToAssets.second;
				for (AAsset* const asset : assets)
				{
					m_assetNameToAssets[assetType][asset->GetAssetName()] = asset;
				}
			}

		}
	}
}

BaseTextureAsset* const AssetManager::GetBaseTextureAsset(const string& assetName)
{
	return GetAssetHelper<BaseTextureAsset>(EAssetType::ASSET_TYPE_BASE_TEXTURE, assetName);
}

ScratchTextureAsset* const  AssetManager::GetScratchTextureAsset(const string& assetName)
{
	return GetAssetHelper<ScratchTextureAsset>(EAssetType::ASSET_TYPE_SCRATCH_TEXTURE, assetName);
}

StaticMeshAsset* const AssetManager::GetStaticMeshAsset(const string& assetName)
{
	return GetAssetHelper<StaticMeshAsset>(EAssetType::ASSET_TYPE_STATIC, assetName);
}

SkeletalMeshAsset* const AssetManager::GetSkeletalMeshAsset(const string& assetName)
{
	return GetAssetHelper<SkeletalMeshAsset>(EAssetType::ASSET_TYPE_SKELETAL, assetName);
}

ModelMaterialAsset* const AssetManager::GetModelMaterialAsset(const string& assetName)
{
	return GetAssetHelper<ModelMaterialAsset>(EAssetType::ASSET_TYPE_MODEL_MATERIAL, assetName);
}

IBLMaterialAsset* const AssetManager::GetIBLMaterialAsset(const string& assetName)
{
	return GetAssetHelper<IBLMaterialAsset>(EAssetType::ASSET_TYPE_IBL_MATERIAL, assetName);
}

BoneAsset* const AssetManager::GetBoneAsset(const string& assetName)
{
	return GetAssetHelper<BoneAsset>(EAssetType::ASSET_TYPE_BONE, assetName);
}

AnimationAsset* const AssetManager::GetAnimationAsset(const string& assetName)
{
	return GetAssetHelper<AnimationAsset>(EAssetType::ASSET_TYPE_ANIMATION, assetName);
}

MapAsset* const AssetManager::GetMapAsset(const string& assetName)
{
	return GetAssetHelper<MapAsset>(EAssetType::ASSET_TYPE_MAP, assetName);
}

BaseTextureAsset* const AssetManager::GetResourceAsset(const string& assetName)
{
	return GetAssetHelper<BaseTextureAsset>(EAssetType::ASSET_TYPE_RESOURCE, assetName);
}


template<typename T>
inline T* const AssetManager::GetAssetHelper(const EAssetType& asssetType, const string& assetName)
{
	const unordered_map<string, AAsset*>& assetNameToAssets = m_assetNameToAssets[asssetType];
	if (assetNameToAssets.find(assetName) != assetNameToAssets.end())
	{
		return dynamic_cast<T*>(assetNameToAssets.at(assetName));
	}
	return nullptr;
}
