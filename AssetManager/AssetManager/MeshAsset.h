#pragma once
#include "Asset.h"
#include "ModelMaterialAsset.h"

namespace DirectX
{
	struct XMFLOAT2;
	struct XMFLOAT3;
}

class MeshAsset : public AAsset
{
public:
	MeshAsset(const std::string& assetNameIn);
	virtual ~MeshAsset();

protected:
	std::vector<DirectX::XMFLOAT3> m_positions;
	std::vector<DirectX::XMFLOAT2> m_uvTextures;
	std::vector<DirectX::XMFLOAT3> m_normals;

protected:
	std::vector<uint32_t> m_indices;

protected:
	std::string m_defaultMaterialName;
	std::shared_ptr<ModelMaterialAsset> m_defaultMaterial;

public:
	void UpdateDefaultMaterialAsset(IModelMaterialProvider& provider);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

