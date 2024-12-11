#include "pch.h"
#include "BoneAsset.h"

using namespace std;
using namespace DirectX;

Bone::~Bone() 
{
	for (Bone* bone : m_boneChildren)
	{
		delete bone;
	}
}

void Bone::SetBoneProperties(const uint32_t& boneIdxIn, const XMMATRIX offsetMatrix)
{
	m_boneIdx = boneIdxIn;
	m_offsetMatrix = offsetMatrix;
}

inline void Bone::SetParentBone(Bone* const parentBone)
{
	m_parentBone = parentBone;
}

inline void Bone::AddChildBone(Bone* const childBone)
{
	childBone->SetParentBone(this);
	m_boneChildren.push_back(childBone);
}

void Bone::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeElement(m_boneIdx, fileIn);
	SerializeHelper::SerializeElement(m_offsetMatrix, fileIn);

}

void Bone::Deserialize(FILE* fileIn)
{
	m_boneIdx = DeserializeHelper::DeserializeElement<uint32_t>(fileIn);
	m_offsetMatrix = DeserializeHelper::DeserializeElement<XMMATRIX>(fileIn);
}

BoneAsset::BoneAsset(const string& assetName)
	: AAsset(assetName)
{
}

BoneAsset::~BoneAsset() 
{
	delete m_rootBone;
}


void BoneAsset::SetRootBone(Bone* const bone)
{
	m_rootBone = bone;
}

void BoneAsset::AddBone(Bone* const bone, const std::string& boneName)
{
	m_boneToNames.emplace(bone, boneName);
}

void BoneAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	function<void(Bone* const)> dfs = [&](Bone* const currentBone)
	{
		SerializeHelper::SerializeString(
			m_boneToNames.find(currentBone) != m_boneToNames.end() ? m_boneToNames.at(currentBone) : "",
			fileIn
		);

		currentBone->Serialize(fileIn);

		const list<Bone*>& boneChildren = currentBone->GetBoneChildren();

		SerializeHelper::SerializeContainerSize(boneChildren, fileIn);
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

	function<Bone*()> dfs = [&]()
	{
		Bone* currentBone = new Bone();

		string currentBoneName = DeserializeHelper::DeserializeString(fileIn);
		currentBone->Deserialize(fileIn);

		m_boneToNames.emplace(currentBone, currentBoneName);

		size_t currentBoneChildrenCount = DeserializeHelper::DeserializeElement<size_t>(fileIn);
		for (size_t childIdx = 0; childIdx < currentBoneChildrenCount; ++childIdx)
		{
			currentBone->AddChildBone(dfs());
		}

		return currentBone;
	};

	m_rootBone = dfs();
}
