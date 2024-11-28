#include "pch.h"
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace DirectX;

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
}

void SkeletalMeshAsset::SetBoneAsset(const std::shared_ptr<BoneAsset>& boneAsset)
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

void SkeletalMeshAsset::Serialize(FILE* fileIn) const
{
	AMeshAsset::Serialize(fileIn);

	SerializeHelper::SerializeContainerSize(m_skeletalMeshPartsPerLOD, fileIn);
	for (auto& skeletalMeshParts : m_skeletalMeshPartsPerLOD)
	{
		skeletalMeshParts.Serialize(fileIn);
	};

	SerializeHelper::SerializeString(m_boneAssetName, fileIn);

}

void SkeletalMeshAsset::Deserialize(FILE* fileIn)
{
	AMeshAsset::Deserialize(fileIn);

	const size_t containerSize = DeserializeHelper::DeserializeContainerSize(fileIn);
	for (size_t idx = 0; idx < containerSize; ++idx)
	{
		SkeletalMeshPartData skeletalMeshPartData;
		skeletalMeshPartData.Deserialize(fileIn);
		m_skeletalMeshPartsPerLOD.emplace_back(skeletalMeshPartData);
	}

	m_boneAssetName = DeserializeHelper::DeserializeString(fileIn);
}

