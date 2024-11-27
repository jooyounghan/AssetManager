#pragma once
#include "MeshSetAsset.h"
#include "StaticMeshAsset.h"

class StaticMeshSetAsset : public MeshSetAsset
{
public:
	StaticMeshSetAsset() = default;
	StaticMeshSetAsset(const std::string& assetName);
	virtual ~StaticMeshSetAsset();

protected:
	std::vector<std::shared_ptr<StaticMeshAsset>> m_staticMeshAssets;

public:
	void UpdateStaticMeshAndMaterialAsset(
		IStaticMeshProvider& staticMeshProvider, 
		IModelMaterialProvider& modelMaterialProvider
	);
};

