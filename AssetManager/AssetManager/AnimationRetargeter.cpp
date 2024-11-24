#include "pch.h"
#include "AnimationRetargeter.h"
#include "AssetException.h"

#include <unordered_set>

using namespace std;
using namespace DirectX;

AnimationRetargeter::AnimationRetargeter()
{
}

AnimationRetargeter::~AnimationRetargeter()
{
}

std::shared_ptr<AnimationAsset> AnimationRetargeter::GetRetargetedAnimation(const std::string& assetNameIn)
{
	return std::shared_ptr<AnimationAsset>();
}

void AnimationRetargeter::GenerateBoneTargetings()
{
	m_boneTargetings.clear();

	if (m_sourceBoneAsset != nullptr && m_destBoneAsset != nullptr)
	{
		const map<shared_ptr<Bone>, string>& sourceBoneToNames = m_sourceBoneAsset->GetBoneToNames();
		const map<shared_ptr<Bone>, string>& destBoneToNames = m_destBoneAsset->GetBoneToNames();

		unordered_map<string, shared_ptr<Bone>> sourceNameToBones;
		for (auto& sourceBoneToName : sourceBoneToNames)
		{
			sourceNameToBones.emplace(sourceBoneToName.second, sourceBoneToName.first);
		}

		for (auto& destBoneToName : destBoneToNames)
		{
			if (sourceNameToBones.find(destBoneToName.second) != sourceNameToBones.end())
			{
				m_boneTargetings.emplace(destBoneToName.first, sourceNameToBones[destBoneToName.second]);
			}
			else
			{
				m_boneTargetings.emplace(destBoneToName.first, nullptr);
			}
		}
	}
}

void AnimationRetargeter::ReplaceTargetedSourceBone(const std::string& destBoneName, const std::string sourceBoneName)
{
}

bool AnimationRetargeter::IsSameProfile(const std::shared_ptr<BoneAsset>& boneAssetIn, const std::shared_ptr<AnimationAsset>& animationAssetIn)
{
	if (boneAssetIn != nullptr)
	{
		return false;
	}
	if (animationAssetIn != nullptr)
	{
		return false;
	}

	const map<shared_ptr<Bone>, string>& boneToNames = boneAssetIn->GetBoneToNames();
	const unordered_map<string, AnimChannel>& boneNameToAnimationChannels = animationAssetIn->GetBoneNameToAnimChannels();

	for (const auto& boneToName : boneToNames)
	{
		if (boneNameToAnimationChannels.find(boneToName.second) == boneNameToAnimationChannels.end())
		{
			return false;
		}
	}

	return true;
}

std::map<std::string, DirectX::XMMATRIX> AnimationRetargeter::GetTPoseLocalTransformations(const std::shared_ptr<BoneAsset>& boneAssetIn)
{
	map<string, XMMATRIX> tPoseLocalTransformation;

	shared_ptr<Bone> rootBone = boneAssetIn->GetRootBone();
	const map<shared_ptr<Bone>, string>& boneToNames = boneAssetIn->GetBoneToNames();

	for (auto& boneToName : boneToNames)
	{
		tPoseLocalTransformation[boneToName.second] = XMMatrixIdentity();
	}

	function<void(const shared_ptr<Bone>&)> updateTLocalTransformation = [&](const shared_ptr<Bone>& currentBone)
		{
			if (currentBone != nullptr)
			{
				const string& boneName = boneToNames.at(currentBone);
				shared_ptr<Bone> parentBone = currentBone->GetParentBone();

				tPoseLocalTransformation[boneName] 
					= XMMatrixMultiply(
						XMMatrixInverse(nullptr, currentBone->GetOffsetMatrix()),
						parentBone != nullptr ? parentBone->GetOffsetMatrix() : XMMatrixIdentity()
					);

				const list<shared_ptr<Bone>> childrenBone = currentBone->GetBoneChildren();
				for (const shared_ptr<Bone>& childBone : childrenBone)
				{
					updateTLocalTransformation(childBone);
				}
			}
		};

	updateTLocalTransformation(rootBone);

	return tPoseLocalTransformation;

}
