#pragma once
#include <memory>
#include "BoneAsset.h"
#include "AnimationAsset.h"

namespace DirectX
{
	struct XMMATRIX;
}

class AnimationRetargeter
{
public:
	AnimationRetargeter();
	~AnimationRetargeter();

protected:
	std::shared_ptr<BoneAsset> m_sourceBoneAsset;
	std::shared_ptr<BoneAsset> m_destBoneAsset;

protected:
	std::shared_ptr<AnimationAsset> m_sourceAnimationAsset;

protected:
	std::unordered_map<std::shared_ptr<Bone>, std::shared_ptr<Bone>> m_boneTargetings;

public:
	std::shared_ptr<AnimationAsset> GetRetargetedAnimation(const std::string& assetNameIn);

public:
	void GenerateBoneTargetings();
	void ReplaceTargetedSourceBone(const std::string& destBoneName, const std::string sourceBoneName);

private:
	bool IsSameProfile(
		const std::shared_ptr<BoneAsset>& boneAssetIn,
		const std::shared_ptr<AnimationAsset>& animationAssetIn
	);

private:
	std::map<std::string, DirectX::XMMATRIX> GetTPoseLocalTransformations(const std::shared_ptr<BoneAsset>& boneAssetIn);
};

