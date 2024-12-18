#pragma once
#include "MeshAssetWriter.h"

class StaticMeshAssetWriter : public MeshAssetWriter
{
public:
	virtual void LoadMeshPartData(
		MeshPartsData* meshPartData,
		const bool& isGltf,
		const aiMesh* const mesh,
		const DirectX::XMMATRIX& transformation
	) override;

public:

};

