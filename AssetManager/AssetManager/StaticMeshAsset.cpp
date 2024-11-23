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

void StaticMeshAsset::Serialize(FILE* fileIn) const
{
	MeshAsset::Serialize(fileIn);
	SerializeHelper::SerializeSequenceContainer(m_tangents, fileIn);
}

void StaticMeshAsset::Deserialize(FILE* fileIn)
{
	MeshAsset::Deserialize(fileIn);
	m_tangents = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT3>>(fileIn);
}

