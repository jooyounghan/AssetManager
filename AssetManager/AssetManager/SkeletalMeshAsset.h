#pragma once
#include "StaticMeshAsset.h"
#include "BoneAsset.h"

class SkeletalMeshAsset : public StaticMeshAsset
{
public:
	SkeletalMeshAsset(const std::string& assetNameIn);
	virtual ~SkeletalMeshAsset();

protected:
	std::vector<DirectX::XMFLOAT4> m_blendWeight;
	std::vector<DirectX::XMFLOAT4> m_blendIndex;

protected:
	std::string m_boneName;
	std::shared_ptr<BoneAsset> m_bone;

public:
	void UpdateBoneAsset(
		IBoneProvider& provider
	);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

