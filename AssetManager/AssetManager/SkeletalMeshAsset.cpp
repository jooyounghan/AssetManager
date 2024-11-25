#include "pch.h"
#include "SkeletalMeshAsset.h"

using namespace std;
using namespace DirectX;

SkeletalMeshAsset::SkeletalMeshAsset(const string& assetNameIn)
	: StaticMeshAsset(assetNameIn)
{
}

SkeletalMeshAsset::~SkeletalMeshAsset()
{
}

void SkeletalMeshAsset::UpdateBoneAsset(IBoneProvider& provider)
{
	m_bone = provider.GetBoneAsset(m_boneName);
}

void SkeletalMeshAsset::Serialize(FILE* fileIn) const
{
	StaticMeshAsset::Serialize(fileIn);

	SerializeHelper::SerializeSequenceContainer(m_blendWeight, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_blendIndex, fileIn);

	SerializeHelper::SerializeString(m_boneName, fileIn);

}

void SkeletalMeshAsset::Deserialize(FILE* fileIn)
{
	StaticMeshAsset::Deserialize(fileIn);

	m_blendWeight = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT4>>(fileIn);
	m_blendIndex = DeserializeHelper::DeserializeSequenceContainer<vector<XMFLOAT4>>(fileIn);

	m_boneName = DeserializeHelper::DeserializeString(fileIn);
}
