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
	BoneAsset* m_sourceBoneAsset = nullptr;
	BoneAsset* m_destBoneAsset = nullptr;

protected:
	AnimationAsset* m_sourceAnimationAsset = nullptr;

protected:
	std::unordered_map<Bone*, Bone*> m_boneTargetings;

public:
	AnimationAsset* GetRetargetedAnimation(const std::string& assetName);

public:
	void GenerateBoneTargetings();
	void ReplaceTargetedSourceBone(Bone* const destBone, Bone* const sourceBone);

private:
	bool IsSameProfile(
		const BoneAsset* const boneAssetIn,
		const AnimationAsset* const animationAssetIn
	);

private:
	std::map<std::string, DirectX::XMMATRIX> GetTPoseLocalTransformations(const BoneAsset* const boneAssetIn);
};

