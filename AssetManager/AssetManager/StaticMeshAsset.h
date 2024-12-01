#pragma once
#include "MeshAsset.h"

class StaticMeshPartData : public MeshPartsData
{
protected:
	std::vector<DirectX::XMFLOAT3> m_tangents;

public:
	void AddTangent(const float& x, const float& y, const float& z);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class StaticMeshAsset : public AMeshAsset
{
public:
	StaticMeshAsset() = default;
	StaticMeshAsset(const std::string& assetName);
	virtual ~StaticMeshAsset();

protected:
	std::map<uint32_t, std::shared_ptr<StaticMeshPartData>> m_staticMeshPartsPerLOD;

public:
	virtual size_t GetLODCount() override;
	virtual std::shared_ptr<MeshPartsData> GetMeshPartData(const uint32_t& lodLevel) override;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;;
};

class IStaticMeshProvider
{
public:
	virtual std::shared_ptr<StaticMeshAsset> GetStaticMeshAsset(
		const std::string& assetName
	) = 0;
};