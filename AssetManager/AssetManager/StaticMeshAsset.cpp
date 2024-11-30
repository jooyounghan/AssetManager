#include "pch.h"
#include "StaticMeshAsset.h"

using namespace std;
using namespace DirectX;

void StaticMeshPartData::AddTangent(const float& x, const float& y, const float& z)
{
	m_tangents.emplace_back(XMFLOAT3(x, y, z));
}

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

shared_ptr<MeshPartsData> StaticMeshAsset::GetMeshPartData(const uint32_t& lodLevel)
{
	if (m_staticMeshPartsPerLOD.find(lodLevel) == m_staticMeshPartsPerLOD.end())
	{
		m_staticMeshPartsPerLOD[lodLevel] = make_shared<StaticMeshPartData>();
	}
	return m_staticMeshPartsPerLOD[lodLevel];
}

void StaticMeshAsset::Serialize(FILE* fileIn) const
{
	AMeshAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_staticMeshPartsPerLOD, fileIn);
	for (auto& staticMeshPart : m_staticMeshPartsPerLOD)
	{
		const uint32_t& lodLevel = staticMeshPart.first;
		const shared_ptr<StaticMeshPartData>& meshPartData = staticMeshPart.second;

		SerializeHelper::SerializeElement(lodLevel, fileIn);
		meshPartData->Serialize(fileIn);
	}
}

void StaticMeshAsset::Deserialize(FILE* fileIn)
{
	AMeshAsset::Deserialize(fileIn);
	
	const size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerSize; ++idx)
	{
		const uint32_t lodLevel = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
		shared_ptr<StaticMeshPartData> staticMeshPartData = make_shared<StaticMeshPartData>();
		staticMeshPartData->Deserialize(fileIn);
		m_staticMeshPartsPerLOD.emplace(lodLevel, staticMeshPartData);
	}
}
