#include "pch.h"
#include "AssetReader.h"

#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"

#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"

#include "ModelMaterialAsset.h"
#include "IBLMaterialAsset.h"

#include "MapAsset.h"

#include <filesystem>

using namespace std;
using namespace std::filesystem;

AssetReader::AssetReader(const string& assetBasePath)
	: m_assetBasePath(assetBasePath)
{
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_STATIC);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_SKELETAL);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_BONE);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_ANIMATION);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_BASE_TEXTURE);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_SCRATCH_TEXTURE);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_MODEL_MATERIAL);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_IBL_MATERIAL);
	m_topologySorter.AddTopologyNode(EAssetType::ASSET_TYPE_MAP);

	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_STATIC, EAssetType::ASSET_TYPE_MODEL_MATERIAL);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_SKELETAL, EAssetType::ASSET_TYPE_MODEL_MATERIAL);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_SKELETAL, EAssetType::ASSET_TYPE_BONE);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_ANIMATION, EAssetType::ASSET_TYPE_BONE);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_MODEL_MATERIAL, EAssetType::ASSET_TYPE_BASE_TEXTURE);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_MODEL_MATERIAL, EAssetType::ASSET_TYPE_SCRATCH_TEXTURE);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_IBL_MATERIAL, EAssetType::ASSET_TYPE_BASE_TEXTURE);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_IBL_MATERIAL, EAssetType::ASSET_TYPE_SCRATCH_TEXTURE);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_MAP, EAssetType::ASSET_TYPE_IBL_MATERIAL);
	m_topologySorter.AddPrequisite(EAssetType::ASSET_TYPE_MAP, EAssetType::ASSET_TYPE_STATIC);


	path assetBasePathSet = path(m_assetBasePath);
	if (!exists(assetBasePathSet) && create_directories(assetBasePathSet)) {/* Do Nothing But Make Directory */ };


	UpdatePreloadArgs();
	GetLoadedAsset();
}

AssetReader::~AssetReader()
{
}

void AssetReader::UpdatePreloadArgs()
{
	m_assetTypeToPreloadArgs.clear();

	for (const auto& entry : recursive_directory_iterator(m_assetBasePath))
	{
		if (entry.is_regular_file())
		{
			const path& assetPath = entry.path();
			if (assetPath.extension().string() == AAsset::AssetExtension)
			{
				AssetWrapper wrapper;
				FILE* fileIn = nullptr;

				const string& assetPathStr = assetPath.string();
				fopen_s(&fileIn, assetPathStr.c_str(), "rb");
				if (fileIn != nullptr)
				{
					wrapper.Deserialize(fileIn);
					const EAssetType assetType = wrapper.GetAssetType();

					SAssetPreloadArgs assetPreloadArgs;
					assetPreloadArgs.assetPath = assetPathStr;
					assetPreloadArgs.lastReadPoint = ftell(fileIn);

					fclose(fileIn);

					m_assetTypeToPreloadArgs[assetType].emplace_back(move(assetPreloadArgs));
				}
			}
		}
	}
}

unordered_map<EAssetType, vector<AAsset*>> AssetReader::GetLoadedAsset() const
{
	function<AAsset*()> assetMaker;
	unordered_map<EAssetType, vector<AAsset*>> loadedAssets;

	vector<EAssetType> loadPriorities = m_topologySorter.GetTopologySort();
	for (const EAssetType& loadPriority : loadPriorities)
	{
		if (m_assetTypeToPreloadArgs.find(loadPriority) != m_assetTypeToPreloadArgs.end())
		{
			vector<AAsset*> loadedAssetsPerType;

			const vector<SAssetPreloadArgs>& preloadArgs = m_assetTypeToPreloadArgs.at(loadPriority);

			switch (loadPriority)
			{
			case EAssetType::ASSET_TYPE_STATIC:
				assetMaker = [&] { return new StaticMeshAsset(); };
				break;
			case EAssetType::ASSET_TYPE_SKELETAL:
				assetMaker = [&] { return new SkeletalMeshAsset(); };
				break;
			case EAssetType::ASSET_TYPE_BONE:
				assetMaker = [&] { return new BoneAsset(); };
				break;
			case EAssetType::ASSET_TYPE_ANIMATION:
				assetMaker = [&] { return new AnimationAsset(); };
				break;
			case EAssetType::ASSET_TYPE_BASE_TEXTURE:
				assetMaker = [&] { return new BaseTextureAsset(); };
				break;
			case EAssetType::ASSET_TYPE_SCRATCH_TEXTURE:
				assetMaker = [&] { return new ScratchTextureAsset(); };
				break;
			case EAssetType::ASSET_TYPE_MODEL_MATERIAL:
				assetMaker = [&] { return new ModelMaterialAsset(); };
				break;
			case EAssetType::ASSET_TYPE_IBL_MATERIAL:
				assetMaker = [&] { return new IBLMaterialAsset(); };
				break;
			case EAssetType::ASSET_TYPE_MAP:
				assetMaker = [&] { return new MapAsset(); };
				break;
			}

			for (const SAssetPreloadArgs& preloadArg : preloadArgs)
			{
				FILE* fileIn = nullptr;
				const string& assetPath = preloadArg.assetPath.c_str();
				const long& lastReadPoint = preloadArg.lastReadPoint;

				fopen_s(&fileIn, assetPath.c_str(), "rb");
				if (fileIn != nullptr)
				{
					AAsset* asset = assetMaker();
					loadedAssetsPerType.push_back(asset);

					fseek(fileIn, lastReadPoint, SEEK_SET);
					asset->Deserialize(fileIn);
					fclose(fileIn);
				}
			}

			loadedAssets.emplace(loadPriority, loadedAssetsPerType);
		}
	}
	return loadedAssets;
}

