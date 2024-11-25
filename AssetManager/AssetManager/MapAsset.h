#pragma once
#include "Asset.h"
#include "IBLMaterialAsset.h"
#include "StaticMeshAsset.h"
#include <map>
class MapAsset : public AAsset
{
public:
	MapAsset(const std::string& assetNameIn);
	virtual ~MapAsset();

protected:
	std::string m_iblMaterialName;
	std::shared_ptr<IBLMaterialAsset> m_iblMaterial;

protected:
	std::unordered_map<std::string, std::shared_ptr<StaticMeshAsset>> m_assetNameToAssets;
	std::unordered_map<std::string, std::vector<DirectX::XMMATRIX>> m_assetNameToTransformations;

public:
	void UpdateIBLMaterialAsset(
		IIBLMaterialProvider& provider
	);
	void UpdateStaticMeshAssets(
		IStaticMeshProvider& provider
	);

public:
	void SetIBLMaterialAsset(
		const std::string& iblmaterialAssetNameIn,
		IIBLMaterialProvider& provider
	);

	void AddStaticMesh(
		const std::string& assetName, 
		const DirectX::XMMATRIX& transformation,
		IStaticMeshProvider& provider
	);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

