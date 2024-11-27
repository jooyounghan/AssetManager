#pragma once
#include "MeshAsset.h"

class StaticMeshAsset : public MeshAsset
{
public:
	StaticMeshAsset() = default;
	StaticMeshAsset(const std::string& assetName);
	virtual ~StaticMeshAsset();

protected:
	std::vector<DirectX::XMFLOAT3> m_tangents;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;;
};

class IStaticMeshProvider
{
public:
	virtual std::shared_ptr<StaticMeshAsset> GetStaticMeshAsset(
		const std::string& textureName
	) = 0;
};