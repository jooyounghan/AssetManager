#include "pch.h"
#include "StaticMeshAsset.h"

using namespace std;
using namespace DirectX;

void StaticMeshPartData::Serialize(FILE* fileIn) const
{
	MeshPartsData::Serialize(fileIn);

	SerializeHelper::SerializeSequenceContainer(m_tangents, fileIn);
}

void StaticMeshPartData::Deserialize(FILE* fileIn)
{
	MeshPartsData::Deserialize(fileIn);

	m_tangents = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT3>>(fileIn);
}


StaticMeshAsset::StaticMeshAsset(const string& assetName)
	: AMeshAsset(assetName)
{

}


StaticMeshAsset::~StaticMeshAsset() 
{

}

size_t StaticMeshAsset::GetLODCount()
{
	return m_staticMeshPartsPerLOD.size();
}

void StaticMeshAsset::Serialize(FILE* fileIn) const
{
	AMeshAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_staticMeshPartsPerLOD, fileIn);
	for (auto& staticMeshParts : m_staticMeshPartsPerLOD)
	{
		staticMeshParts.Serialize(fileIn);
	}
}

void StaticMeshAsset::Deserialize(FILE* fileIn)
{
	AMeshAsset::Deserialize(fileIn);
	
	const size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerSize; ++idx)
	{
		StaticMeshPartData staticMeshPartData;
		staticMeshPartData.Deserialize(fileIn);
		m_staticMeshPartsPerLOD.emplace_back(staticMeshPartData);
	}
}
