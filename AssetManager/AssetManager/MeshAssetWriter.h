#pragma once
#include "MeshAsset.h"

struct aiMesh;

class MeshAssetWriter
{
public:
	virtual void LoadMeshPartData(
		std::shared_ptr<MeshPartsData> meshPartData,
		const bool& isGltf,
		const aiMesh* const mesh,
		const DirectX::XMMATRIX& transformation
	);
};

