#pragma once
#include "Asset.h"
#include <DirectXMath.h>

#include <map>
#include <list>
#include <memory>

class Bone : public ISerializable
{
public:
	Bone() = default;
	~Bone();

private:
	uint32_t m_boneIdx = NULL;
	DirectX::XMMATRIX m_offsetMatrix = DirectX::XMMatrixIdentity();
	Bone* m_parentBone = nullptr;
	std::list<Bone*> m_boneChildren;

public:
	MakeGetter(m_offsetMatrix, OffsetMatrix);
	MakeGetter(m_parentBone, ParentBone);
	MakeGetter(m_boneChildren, BoneChildren);

public:
	void SetBoneProperties(const uint32_t& boneIdxIn, const DirectX::XMMATRIX offsetMatrix);
	void SetParentBone(Bone* const parentBone);
	void AddChildBone(Bone* const childBone);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class BoneAsset : public AAsset
{
public:
	BoneAsset() = default;
	BoneAsset(const std::string& assetName);
	virtual ~BoneAsset();

protected:
	Bone* m_rootBone = nullptr;
	std::map<Bone*, std::string> m_boneToNames;

public:
	MakeGetter(m_rootBone, RootBone);
	MakeGetter(m_boneToNames, BoneToNames);

public:
	void SetRootBone(Bone* const bone);
	void AddBone(Bone* const bone, const std::string& boneName);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class IBoneProvider
{
public:
	virtual BoneAsset* const GetBoneAsset(
		const std::string& textureName
	) = 0;
};

