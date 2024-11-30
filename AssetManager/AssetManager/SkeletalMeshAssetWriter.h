#pragma once
#include "StaticMeshAssetWriter.h"

class SkeletalMeshAssetWriter : public StaticMeshAssetWriter
{
public:
	virtual void LoadMeshPartData(
		std::shared_ptr<MeshPartsData> meshPartData,
		const bool& isGltf,
		const aiMesh* const mesh,
		const DirectX::XMMATRIX& transformation
	) override;
};

