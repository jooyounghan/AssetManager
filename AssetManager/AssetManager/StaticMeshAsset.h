#pragma once
#include "MeshAsset.h"

constexpr const char* StaticMeshAssetOutPath = ".\\Assets\\StaticMesh\\";

class StaticMeshAsset : public MeshAsset
{
public:
	StaticMeshAsset(const std::string& assetPathIn, const std::string& assetNameIn);
	virtual ~StaticMeshAsset();;

protected:
	std::vector<DirectX::XMFLOAT3> m_tangents;

public:
	virtual void Serialize(FILE* fileIn) override;
	virtual void Deserialize(FILE* fileIn) override;;
};

