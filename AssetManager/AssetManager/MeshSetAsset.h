#pragma once
#include "Asset.h"
#include "MeshAsset.h"
#include "ModelMaterialAsset.h"

class MeshSetAsset : public AAsset
{
public:
	MeshSetAsset() = default;
	MeshSetAsset(const std::string& assetName);
	virtual ~MeshSetAsset();

protected:
	std::unordered_map<std::string, std::string> m_meshAssetNamesToModelMaterialNames;
	std::vector<std::shared_ptr<ModelMaterialAsset>> m_modelMaterialAssets;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

