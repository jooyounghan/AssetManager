#pragma once
#include "MeshSetAsset.h"
#include "SkeletalMeshAsset.h"

class SkeletalMeshSetAsset : public MeshSetAsset
{
public:
	SkeletalMeshSetAsset() = default;
	SkeletalMeshSetAsset(const std::string& assetName);
	virtual ~SkeletalMeshSetAsset();

protected:
	std::vector<std::shared_ptr<SkeletalMeshAsset>> m_skeletalMeshAssets;

public:
	void UpdateSkeletalMeshAndMaterialAsset(
		ISkeletalMeshProvider& skeletalMeshProvider,
		IModelMaterialProvider& modelMaterialProvider
	);
};

