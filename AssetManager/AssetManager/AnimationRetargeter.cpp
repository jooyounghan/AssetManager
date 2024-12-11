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

AnimationAsset* AnimationRetargeter::GetRetargetedAnimation(const std::string& assetName)
{
	if (m_boneTargetings.empty())
	{
		return nullptr;
	}
	else if (m_sourceBoneAsset == nullptr || m_destBoneAsset == nullptr)
	{
		return nullptr;
	}
	else if (m_sourceAnimationAsset == nullptr)
	{
		return nullptr;
	}
	else
	{
		const float& Duration = m_sourceAnimationAsset->GetDuration();
		const float& TicksPerSecond = m_sourceAnimationAsset->GetTicksPerSecond();

		const unordered_map<string, AnimChannel>& SourceBoneNameToAnimChannels = m_sourceAnimationAsset->GetBoneNameToAnimChannels();
		AnimationAsset* RetargetedAnimation = new AnimationAsset(assetName);

		const map<string, XMMATRIX> TPoseLocalSourceTransformations = GetTPoseLocalTransformations(m_sourceBoneAsset);
		const map<string, XMMATRIX> TPoseLocalDestTransformations = GetTPoseLocalTransformations(m_destBoneAsset);

		const map<Bone*, string>& SourceBoneToNames = m_sourceBoneAsset->GetBoneToNames();
		const map<Bone*, string>& DestBoneToNames = m_destBoneAsset->GetBoneToNames();

		for (const auto& BoneTargeting : m_boneTargetings)
		{
			Bone* const DestBone = BoneTargeting.first;
			Bone* const SourceBone = BoneTargeting.second;

			const string& DestBoneName = DestBoneToNames.at(DestBone);
			const string& SourceBoneName = SourceBoneToNames.at(SourceBone);

			XMVECTOR Position;
			XMVECTOR Quaternion;
			XMVECTOR Scale;

			if (SourceBoneNameToAnimChannels.find(SourceBoneName) != SourceBoneNameToAnimChannels.end())
			{
				AnimChannel NewChannel;
				const AnimChannel& SourceBoneChannel = SourceBoneNameToAnimChannels.at(SourceBoneName);

				set<float> TimeTable = SourceBoneChannel.GetTimeTable();
				for (const float& Time : TimeTable)
				{
					XMMATRIX SourceLocalAnimation = SourceBoneChannel.GetLocalTransformation(Time);
					const XMMATRIX InvSourceTPose = XMMatrixInverse(nullptr, TPoseLocalSourceTransformations.at(SourceBoneName));
					const XMMATRIX DestTPose = TPoseLocalDestTransformations.at(DestBoneName);


					XMMatrixDecompose(&Scale, &Quaternion, &Position, SourceLocalAnimation * InvSourceTPose * DestTPose);

					NewChannel.AddPositionKey(Time, Position);
					NewChannel.AddQuaternionKey(Time, Quaternion);
					NewChannel.AddScaleKey(Time, Scale);
				}
				RetargetedAnimation->AddAnimChannel(DestBoneName, move(NewChannel));
			}
			else
			{
				const XMMATRIX DestTPose = TPoseLocalDestTransformations.at(DestBoneName);
				XMMatrixDecompose(&Scale, &Quaternion, &Position, DestTPose);

				AnimChannel IdentityChannel;
				IdentityChannel.AddPositionKey(0.f, Position);
				IdentityChannel.AddPositionKey(Duration, Position);
				IdentityChannel.AddQuaternionKey(0.f, Quaternion);
				IdentityChannel.AddQuaternionKey(Duration, Quaternion);
				IdentityChannel.AddScaleKey(0.f, Scale);
				IdentityChannel.AddScaleKey(Duration, Scale);

				RetargetedAnimation->AddAnimChannel(DestBoneName, move(IdentityChannel));
			}
		}

		return RetargetedAnimation;
	}
	return nullptr;
}

void AnimationRetargeter::GenerateBoneTargetings()
{
	m_boneTargetings.clear();

	if (m_sourceBoneAsset != nullptr && m_destBoneAsset != nullptr)
	{
		const map<Bone*, string>& sourceBoneToNames = m_sourceBoneAsset->GetBoneToNames();
		const map<Bone*, string>& destBoneToNames = m_destBoneAsset->GetBoneToNames();

		unordered_map<string, Bone*> sourceNameToBones;
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

void AnimationRetargeter::ReplaceTargetedSourceBone(Bone* const destBone, Bone* const sourceBone)
{
	if (m_boneTargetings.find(destBone) != m_boneTargetings.end())
	{
		m_boneTargetings[destBone] = sourceBone;
	}
}

bool AnimationRetargeter::IsSameProfile(const BoneAsset* const boneAssetIn, const AnimationAsset* const animationAssetIn)
{
	if (boneAssetIn == nullptr)
	{
		return false;
	}
	if (animationAssetIn == nullptr)
	{
		return false;
	}

	const map<Bone*, string>& boneToNames = boneAssetIn->GetBoneToNames();
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

map<string, XMMATRIX> AnimationRetargeter::GetTPoseLocalTransformations(const BoneAsset* const boneAssetIn)
{
	map<string, XMMATRIX> tPoseLocalTransformation;

	Bone* rootBone = boneAssetIn->GetRootBone();
	const map<Bone*, string>& boneToNames = boneAssetIn->GetBoneToNames();

	for (auto& boneToName : boneToNames)
	{
		tPoseLocalTransformation[boneToName.second] = XMMatrixIdentity();
	}

	function<void(Bone* const)> updateTLocalTransformation = [&](Bone* const currentBone)
		{
			if (currentBone != nullptr)
			{
				const string& boneName = boneToNames.at(currentBone);
				Bone* parentBone = currentBone->GetParentBone();

				tPoseLocalTransformation[boneName] 
					= XMMatrixMultiply(
						XMMatrixInverse(nullptr, currentBone->GetOffsetMatrix()),
						parentBone != nullptr ? parentBone->GetOffsetMatrix() : XMMatrixIdentity()
					);

				const list<Bone*> childrenBone = currentBone->GetBoneChildren();
				for (Bone* const childBone : childrenBone)
				{
					updateTLocalTransformation(childBone);
				}
			}
		};

	updateTLocalTransformation(rootBone);

	return tPoseLocalTransformation;

}
