#pragma once
#include "Asset.h"
#include "ModelMaterialAsset.h"
#include <map>

class MeshPartsData : public ISerializable
{
public:
	MeshPartsData() = default;

protected:
	std::vector<DirectX::XMFLOAT3> m_positions;
	std::vector<DirectX::XMFLOAT2> m_uvTextures;
	std::vector<DirectX::XMFLOAT3> m_normals;
	const DirectX::XMFLOAT3 m_emptyFloat3 = DirectX::XMFLOAT3(0, 0, 0);
	const DirectX::XMFLOAT2 m_emptyFloat2 = DirectX::XMFLOAT2(0, 0);

protected:
	std::map<uint32_t, std::vector<uint32_t>> m_offsetToIndices;
	const std::vector<uint32_t> m_emptyIndices;

public:
	const std::vector<uint32_t>& GetIndices(const uint32_t& offset);
	size_t GetPartsCount() { return m_offsetToIndices.size(); }

public:
	const DirectX::XMFLOAT3& GetPosition(const size_t& index);
	const DirectX::XMFLOAT2& GetUVTextureCoord(const size_t& index);
	const DirectX::XMFLOAT3& GetNormal(const size_t& index);

public:
	void AddPosition(const float& x, const float& y, const float& z);
	void AddUVTexture(const float& u, const float& v);
	void AddNormal(const float& x, const float& y, const float& z);
	void AddIndex(const uint32_t& offset, const uint32_t index);
	uint32_t GetVerticesCount();
	std::vector<uint32_t> GetVertexOffsets();

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
	std::vector<ModelMaterialAsset*> m_defaultMaterials;

public:
	void UpdateDefaultMaterialAsset(IModelMaterialProvider& provider);

public:
	virtual size_t GetLODCount() = 0;
	virtual MeshPartsData* GetMeshPartData(const uint32_t& lodLevel) = 0;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

