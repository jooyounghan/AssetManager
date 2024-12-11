#include "pch.h"
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace DirectX;

void SkeletalMeshPartData::AppendBlendProperties(const size_t& appendCount)
{
	m_blendWeight.resize(m_blendWeight.size() + appendCount);
	m_blendIndex.resize(m_blendIndex.size() + appendCount);
}

void SkeletalMeshPartData::SetBlendProperties(
	const size_t& vertexIndex, 
	const int& boneIndex,
	const float& blendWeight
)
{
	if (m_blendWeight.size() <= vertexIndex) return;
	if (m_blendIndex.size() <= vertexIndex) return;

	XMFLOAT4& CurrentBlendWeight = m_blendWeight[vertexIndex];
	XMINT4& CurrentBlendIndex = m_blendIndex[vertexIndex];

	if (CurrentBlendIndex.x == -1)
	{
		CurrentBlendIndex.x = boneIndex; CurrentBlendWeight.x = blendWeight;
	}
	else if (CurrentBlendIndex.y == -1)
	{
		CurrentBlendIndex.y = boneIndex; CurrentBlendWeight.y = blendWeight;
	}
	else if (CurrentBlendIndex.z == -1)
	{
		CurrentBlendIndex.z = boneIndex; CurrentBlendWeight.z = blendWeight;
	}
	else if (CurrentBlendIndex.w == -1)
	{
		CurrentBlendIndex.w = boneIndex; CurrentBlendWeight.w = blendWeight;
	}
	else
	{

	}
}

void SkeletalMeshPartData::Serialize(FILE* fileIn) const
{
	StaticMeshPartData::Serialize(fileIn);

	SerializeHelper::SerializeSequenceContainer(m_blendWeight, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_blendIndex, fileIn);
}

void SkeletalMeshPartData::Deserialize(FILE* fileIn)
{
	StaticMeshPartData::Deserialize(fileIn);

	m_blendWeight = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT4>>(fileIn);
	m_blendIndex = DeserializeHelper::DeserializeSequenceContainer<vector<XMINT4>>(fileIn);
}


SkeletalMeshAsset::SkeletalMeshAsset(const string& assetName)
	: AMeshAsset(assetName)
{
}

SkeletalMeshAsset::~SkeletalMeshAsset()
{
	for (auto& skeletalMeshPartsPerLOD : m_skeletalMeshPartsPerLOD)
	{
		delete skeletalMeshPartsPerLOD.second;
	}
}

void SkeletalMeshAsset::SetBoneAsset(const BoneAsset* const boneAsset)
{
	m_boneAssetName = boneAsset->GetAssetName();
	m_boneAsset = boneAsset;
}

void SkeletalMeshAsset::UpdateBoneAsset(IBoneProvider& provider)
{
	m_boneAsset = provider.GetBoneAsset(m_boneAssetName);
}

size_t SkeletalMeshAsset::GetLODCount()
{
	return m_skeletalMeshPartsPerLOD.size();
}

MeshPartsData* SkeletalMeshAsset::GetMeshPartData(const uint32_t& lodLevel)
{
	if (m_skeletalMeshPartsPerLOD.find(lodLevel) == m_skeletalMeshPartsPerLOD.end())
	{
		m_skeletalMeshPartsPerLOD[lodLevel] = new SkeletalMeshPartData();
	}
	return m_skeletalMeshPartsPerLOD[lodLevel];
}

void SkeletalMeshAsset::Serialize(FILE* fileIn) const
{
	AMeshAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_skeletalMeshPartsPerLOD, fileIn);
	for (auto& skeletalMeshPart : m_skeletalMeshPartsPerLOD)
	{
		const uint32_t& lodLevel = skeletalMeshPart.first;
		const SkeletalMeshPartData* const meshPartData = skeletalMeshPart.second;

		SerializeHelper::SerializeElement(lodLevel, fileIn);
		meshPartData->Serialize(fileIn);
	};

	SerializeHelper::SerializeString(m_boneAssetName, fileIn);

}

void SkeletalMeshAsset::Deserialize(FILE* fileIn)
{
	AMeshAsset::Deserialize(fileIn);

	const size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerSize; ++idx)
	{
		const uint32_t lodLevel = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
		SkeletalMeshPartData* skeletalMeshPartData = new SkeletalMeshPartData();
		skeletalMeshPartData->Deserialize(fileIn);
		m_skeletalMeshPartsPerLOD.emplace(lodLevel, skeletalMeshPartData);
	}

	m_boneAssetName = DeserializeHelper::DeserializeString(fileIn);
}

