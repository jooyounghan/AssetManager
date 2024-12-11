#pragma once
#include "StaticMeshAssetWriter.h"

class SkeletalMeshAssetWriter : public StaticMeshAssetWriter
{
public:
	virtual void LoadMeshPartData(
		MeshPartsData* meshPartData,
		const bool& isGltf,
		const aiMesh* const mesh,
		const DirectX::XMMATRIX& transformation
	) override;
};

