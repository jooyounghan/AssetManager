#include "pch.h"
#include "StaticMeshAssetWriter.h"
#include "StaticMeshAsset.h"
#include "MathematicalHelper.h"

using namespace std;
using namespace DirectX;

void StaticMeshAssetWriter::LoadMeshPartData(
	shared_ptr<MeshPartsData> meshPartData, 
	const bool& isGltf, 
	const aiMesh* const mesh,
	const XMMATRIX& transformation)
{
	MeshAssetWriter::LoadMeshPartData(meshPartData, isGltf, mesh, transformation);
	shared_ptr<StaticMeshPartData> staticMeshPartData = dynamic_pointer_cast<StaticMeshPartData>(meshPartData);
	if (staticMeshPartData != nullptr)
	{
		if (mesh->HasTangentsAndBitangents())
		{
			for (size_t vertexIdx = 0; vertexIdx < mesh->mNumVertices; ++vertexIdx)
			{
				const aiVector3D& tangent = mesh->mTangents[vertexIdx];
				staticMeshPartData->AddTangent(tangent.x, tangent.y, tangent.z);
			}
		}
		else
		{
            const vector<uint32_t> offsets = meshPartData->GetVertexOffsets();
            const uint32_t offset = offsets.empty() ? 0 : offsets[offsets.size() - 1];

            const vector<uint32_t>& indices = meshPartData->GetIndices(offset);
            for (size_t idx = 0; idx < indices.size(); idx += 3)
            {
				const uint32_t& index0 = indices[idx];
				const uint32_t& index1 = indices[idx + 1];
				const uint32_t& index2 = indices[idx + 2];

                const XMFLOAT3& p0 = staticMeshPartData->GetPosition(index0);
				const XMFLOAT3& p1 = staticMeshPartData->GetPosition(index1);
				const XMFLOAT3& p2 = staticMeshPartData->GetPosition(index2);

                const XMFLOAT2& uv0 = staticMeshPartData->GetUVTextureCoord(index0);
                const XMFLOAT2& uv1 = staticMeshPartData->GetUVTextureCoord(index1);
                const XMFLOAT2& uv2 = staticMeshPartData->GetUVTextureCoord(index2);

				const XMFLOAT3& n0 = staticMeshPartData->GetNormal(index0);
				const XMFLOAT3& n1 = staticMeshPartData->GetNormal(index1);
				const XMFLOAT3& n2 = staticMeshPartData->GetNormal(index2);

				XMFLOAT3 t0, t1, t2;
				XMFLOAT3 b0, b1, b2;

				MathematicalHelper::GetTangentBitangent(
					p0, p1, p2,
					uv0, uv1, uv2,
					n0, n1, n2,
					t0, t1, t2,
					b0, b1, b2
				);

				staticMeshPartData->AddTangent(t0.x, t0.y, t0.z);
				staticMeshPartData->AddTangent(t1.x, t1.y, t1.z);
				staticMeshPartData->AddTangent(t2.x, t2.y, t2.z);
            }
		}
	}
}
