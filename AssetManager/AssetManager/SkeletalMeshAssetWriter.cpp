#include "pch.h"
#include "SkeletalMeshAssetWriter.h"
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace DirectX;

void SkeletalMeshAssetWriter::LoadMeshPartData(
	MeshPartsData* meshPartData, 
	const bool& isGltf, 
    const aiMesh* const mesh,
	const XMMATRIX& transformation
)
{
	StaticMeshAssetWriter::LoadMeshPartData(meshPartData, isGltf, mesh, transformation);
	SkeletalMeshPartData* skeletalMeshPartData = dynamic_cast<SkeletalMeshPartData*>(meshPartData);

	if (skeletalMeshPartData != nullptr)
	{
        const vector<uint32_t> offsets = skeletalMeshPartData->GetVertexOffsets();
        const uint32_t offset = offsets.empty() ? 0 : offsets[offsets.size() - 1];
        const uint32_t verticesCount = skeletalMeshPartData->GetVerticesCount();
        const uint32_t newVerticesCount = verticesCount - offset;

        skeletalMeshPartData->AppendBlendProperties(newVerticesCount);
        
        for (uint32_t boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
        {
            aiBone* currentBone = mesh->mBones[boneIdx];
            for (uint32_t weightIdx = 0; weightIdx < currentBone->mNumWeights; ++weightIdx)
            {
                aiVertexWeight& vertexWeight = currentBone->mWeights[weightIdx];

                skeletalMeshPartData->SetBlendProperties(
                    offset + vertexWeight.mVertexId, 
                    boneIdx, 
                    vertexWeight.mWeight
                );
            }
        }
	}
}
