#pragma once
#include "MeshAsset.h"

struct aiMesh;

class MeshAssetWriter
{
public:
	virtual void LoadMeshPartData(
		MeshPartsData* meshPartData,
		const bool& isGltf,
		const aiMesh* const mesh,
		const DirectX::XMMATRIX& transformation
	);
};

