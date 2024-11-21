#include "pch.h"
#include "BoneAsset.h"

using namespace std;
using namespace DirectX;

Bone::Bone() 
	: m_offsetMatrix(XMMatrixIdentity())
{}

Bone::~Bone() {}

void Bone::SetBoneProperties(const size_t boneIdxIn, const XMMATRIX offsetMatrix)
{
	m_boneIdx = boneIdxIn;
	m_offsetMatrix = offsetMatrix;
}

inline void Bone::SetParentBone(const shared_ptr<Bone>& parentBone)
{
	m_parentBone = parentBone;
}

inline void Bone::AddChildBone(const shared_ptr<Bone>& childBone)
{
	childBone->SetParentBone(shared_from_this());
	m_boneChildren.push_back(childBone);
}

void Bone::Serialize(FILE* fileIn)
{
	SerializeHelper::SerializeElement(m_boneIdx, fileIn);
	SerializeHelper::SerializeElement(m_offsetMatrix, fileIn);

}

void Bone::Deserialize(FILE* fileIn)
{
	SerializeHelper::DeserializeElement(m_boneIdx, fileIn);
	SerializeHelper::DeserializeElement(m_offsetMatrix, fileIn);
}

BoneAsset::BoneAsset(
	const string& assetPathIn, 
	const string& assetNameIn
)
	: AAsset(assetPathIn, assetNameIn)
{
}

BoneAsset::~BoneAsset() {}


void BoneAsset::Serialize(FILE* fileIn)
{
	AAsset::Serialize(fileIn);

	function<void(const shared_ptr<Bone>&)> dfs = [&](const shared_ptr<Bone>& currentBone)
	{
		SerializeHelper::SerializeString(
			m_boneToNames.find(currentBone) != m_boneToNames.end() ? m_boneToNames[currentBone] : "",
			fileIn
		);

		currentBone->Serialize(fileIn);

		const list<shared_ptr<Bone>>& boneChildren = currentBone->GetBoneChildren();

		SerializeHelper::SerializeContainerSizeHelper(boneChildren, fileIn);
		for (const auto& bone : boneChildren)
		{
			dfs(bone);
		}
	};
	
	dfs(m_rootBone);
}

void BoneAsset::Deserialize(FILE* fileIn) 
{
	AAsset::Deserialize(fileIn);
}
