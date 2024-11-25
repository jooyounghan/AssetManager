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

void Bone::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeElement(m_boneIdx, fileIn);
	SerializeHelper::SerializeElement(m_offsetMatrix, fileIn);

}

void Bone::Deserialize(FILE* fileIn)
{
	m_boneIdx = DeserializeHelper::DeserializeElement<size_t>(fileIn);
	m_offsetMatrix = DeserializeHelper::DeserializeElement<XMMATRIX>(fileIn);
}

BoneAsset::BoneAsset(const string& assetNameIn)
	: AAsset(assetNameIn)
{
}

BoneAsset::~BoneAsset() {}


void BoneAsset::AttachBone(
	const shared_ptr<Bone>& parentBone,
	const std::string& childBoneName,
	const shared_ptr<Bone>& childBone
)
{
	m_boneToNames.emplace(childBone, childBoneName);
	parentBone->AddChildBone(childBone);
}

void BoneAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	function<void(const shared_ptr<Bone>&)> dfs = [&](const shared_ptr<Bone>& currentBone)
	{
		SerializeHelper::SerializeString(
			m_boneToNames.find(currentBone) != m_boneToNames.end() ? m_boneToNames.at(currentBone) : "",
			fileIn
		);

		currentBone->Serialize(fileIn);

		const list<shared_ptr<Bone>>& boneChildren = currentBone->GetBoneChildren();

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

	function<shared_ptr<Bone>()> dfs = [&]()
	{
		shared_ptr<Bone> currentBone = make_shared<Bone>();

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
