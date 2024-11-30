#include "pch.h"
#include "MeshAssetWriter.h"

using namespace std;
using namespace DirectX;

void MeshAssetWriter::LoadMeshPartData(
	shared_ptr<MeshPartsData> meshPartData, 
	const bool& isGltf,
	const aiMesh* const mesh, 
	const XMMATRIX& transformation
)
{
	if (mesh->HasFaces())
	{
		const uint32_t indexOffset = meshPartData->GetVerticesCount();

		for (uint32_t faceIdx = 0; faceIdx < mesh->mNumFaces; ++faceIdx)
		{
			aiFace& currentFace = mesh->mFaces[faceIdx];

			for (uint32_t idx = 0; idx < currentFace.mNumIndices; ++idx)
			{
				meshPartData->AddIndex(indexOffset, static_cast<uint32_t>(currentFace.mIndices[idx]));
			}
		}
	}

	if (mesh->HasPositions())
	{
		for (uint32_t vertexIdx = 0; vertexIdx < mesh->mNumVertices; ++vertexIdx)
		{
			const aiVector3D& vertex = mesh->mVertices[vertexIdx];
			DirectX::XMVECTOR transformedVertex = DirectX::XMVectorSet(vertex.x, vertex.y, vertex.z, 0.f);
			transformedVertex = DirectX::XMVector3TransformCoord(transformedVertex, transformation);
			meshPartData->AddPosition(transformedVertex.m128_f32[0], transformedVertex.m128_f32[1], transformedVertex.m128_f32[2]);
		}
	}

	if (mesh->HasTextureCoords(0))
	{
		for (size_t vertexIdx = 0; vertexIdx < mesh->mNumVertices; ++vertexIdx)
		{
			const aiVector3D& uvCoordinate = mesh->mTextureCoords[0][vertexIdx];
			meshPartData->AddUVTexture(uvCoordinate.x, uvCoordinate.y);
		}
	}

	if (mesh->HasNormals())
	{
		const float zMultiplier = isGltf ? -1.0f : 1.0f;
		for (uint32_t vertexIdx = 0; vertexIdx < mesh->mNumVertices; ++vertexIdx)
		{
			const aiVector3D& normal = mesh->mVertices[vertexIdx];
			meshPartData->AddNormal(normal.x, normal.y, normal.z * zMultiplier);
		}
	}


}
