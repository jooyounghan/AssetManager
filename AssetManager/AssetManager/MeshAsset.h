#pragma once
#include "Asset.h"
#include "ModelMaterialAsset.h"
#include <map>

namespace DirectX
{
	struct XMFLOAT2;
	struct XMFLOAT3;
}

class MeshPartsData : public ISerializable
{
protected:
	std::vector<DirectX::XMFLOAT3> m_positions;
	std::vector<DirectX::XMFLOAT2> m_uvTextures;
	std::vector<DirectX::XMFLOAT3> m_normals;

protected:
	std::map<uint32_t, std::vector<uint32_t>> m_offsetToIndices;

public:
	size_t GetPartsCount() { return m_offsetToIndices.size(); }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class AMeshAsset : public AAsset
{
public:
	AMeshAsset() = default;
	AMeshAsset(const std::string& assetName);
	virtual ~AMeshAsset();

protected:
	std::vector<std::string> m_defaultMaterialNames;
	std::vector<std::shared_ptr<ModelMaterialAsset>> m_defaultMaterials;

public:
	void UpdateDefaultMaterialAsset(IModelMaterialProvider& provider);

public:
	virtual size_t GetLODCount() = 0;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

