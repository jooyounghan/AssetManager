#include "pch.h"
#include "SkeletalMeshSetAsset.h"

using namespace std;

SkeletalMeshSetAsset::SkeletalMeshSetAsset(const std::string& assetName)
{
}

SkeletalMeshSetAsset::~SkeletalMeshSetAsset()
{
}

void SkeletalMeshSetAsset::UpdateSkeletalMeshAndMaterialAsset(
	ISkeletalMeshProvider& skeletalMeshProvider, 
	IModelMaterialProvider& modelMaterialProvider
)
{
	m_skeletalMeshAssets.clear();
	m_modelMaterialAssets.clear();

	for (auto& staticMeshAssetNamesToModelMaterialName : m_meshAssetNamesToModelMaterialNames)
	{
		const string& skeletalMeshAssetName = staticMeshAssetNamesToModelMaterialName.first;
		const string& modelMaterialAssetName = staticMeshAssetNamesToModelMaterialName.second;

		m_skeletalMeshAssets.emplace_back(skeletalMeshProvider.GetSkeletalMeshAsset(skeletalMeshAssetName));
		m_modelMaterialAssets.emplace_back(modelMaterialProvider.GetModelMaterialAsset(modelMaterialAssetName));
	}
}

