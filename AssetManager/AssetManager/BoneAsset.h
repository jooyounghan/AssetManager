#pragma once
#include "Asset.h"
#include "DirectXMath/DirectXMath.h"

#include <map>
#include <list>
#include <memory>


class Bone : public std::enable_shared_from_this<Bone>, public ISerializable
{
public:
	Bone();
	~Bone();

private:
	size_t m_boneIdx = NULL;
	DirectX::XMMATRIX m_offsetMatrix;
	std::shared_ptr<Bone> m_parentBone = nullptr;
	std::list<std::shared_ptr<Bone>> m_boneChildren;

public:
	const std::list<std::shared_ptr<Bone>>& GetBoneChildren() { return m_boneChildren; }

public:
	void SetBoneProperties(const size_t boneIdxIn, const DirectX::XMMATRIX offsetMatrix);
	void SetParentBone(const std::shared_ptr<Bone>& parentBone);
	void AddChildBone(const std::shared_ptr<Bone>& childBone);

public:
	virtual void Serialize(FILE* fileIn) override;
	virtual void Deserialize(FILE* fileIn) override;
};

class BoneAsset : public AAsset
{
public:
	BoneAsset(const std::string& assetPathIn, const std::string& assetNameIn);
	virtual ~BoneAsset();

protected:
	std::shared_ptr<Bone> m_rootBone = nullptr;
	std::shared_ptr<Bone> m_currentBone = nullptr;

protected:
	std::map<std::shared_ptr<Bone>, std::string> m_boneToNames;

public:
	void AttachBoneToCurrentBone(
		const std::string& boneName,
		const size_t& boneIdxIn,
		const DirectX::XMMATRIX& boneOffsetMatrix
	);

public:
	virtual void Serialize(FILE* fileIn) override;
	virtual void Deserialize(FILE* fileIn) override;;
};

class IBoneProvider
{
public:
	virtual std::shared_ptr<BoneAsset> GetBoneAsset(
		const std::string& textureName
	) = 0;
};

