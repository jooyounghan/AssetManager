#include "pch.h"
#include "StaticMeshAsset.h"

using namespace std;
using namespace DirectX;

StaticMeshAsset::StaticMeshAsset(
	const string& assetPathIn, 
	const string& assetNameIn
)
	: MeshAsset(assetPathIn, assetNameIn)
{

}

StaticMeshAsset::~StaticMeshAsset() 
{

}

void StaticMeshAsset::Serialize(FILE* fileIn)
{
	MeshAsset::Serialize(fileIn);
	SerializeHelper::SerializeVectorHelper(m_tangents, fileIn);
}

void StaticMeshAsset::Deserialize(FILE* fileIn)
{
	MeshAsset::Deserialize(fileIn);
	m_tangents = SerializeHelper::DeserializeVectorHelper<XMFLOAT3>(fileIn);
}

