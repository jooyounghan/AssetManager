#pragma once
#include "Asset.h"
#include "DirectXMath/DirectXMath.h"

#include <map>
#include <list>
#include <memory>

class Bone : public std::enable_shared_from_this<Bone>, public ISerializable
{
public:
	Bone() = default;
	~Bone();

private:
	uint32_t m_boneIdx = NULL;
	DirectX::XMMATRIX m_offsetMatrix = DirectX::XMMatrixIdentity();
	std::shared_ptr<Bone> m_parentBone = nullptr;
	std::list<std::shared_ptr<Bone>> m_boneChildren;

public:
	MakeGetter(m_offsetMatrix, OffsetMatrix);
	MakeGetter(m_parentBone, ParentBone);
	MakeGetter(m_boneChildren, BoneChildren);

public:
	void SetBoneProperties(const uint32_t& boneIdxIn, const DirectX::XMMATRIX offsetMatrix);
	void SetParentBone(const std::shared_ptr<Bone>& parentBone);
	void AddChildBone(const std::shared_ptr<Bone>& childBone);

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
	std::shared_ptr<Bone> m_rootBone = nullptr;
	std::map<std::shared_ptr<Bone>, std::string> m_boneToNames;

public:
	MakeGetter(m_rootBone, RootBone);
	MakeGetter(m_boneToNames, BoneToNames);

public:
	void SetRootBone(const std::shared_ptr<Bone> bone);
	void AddBone(const std::shared_ptr<Bone>& bone, const std::string& boneName);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class IBoneProvider
{
public:
	virtual std::shared_ptr<BoneAsset> GetBoneAsset(
		const std::string& textureName
	) = 0;
};

