#pragma once
#include "Asset.h"
#include "DirectXMath/DirectXMath.h"

#include <functional>
#include <unordered_map>
#include <set>

struct SAnimationKey
{
	float time;
	DirectX::XMVECTOR animationData;
};

class AnimChannel : public ISerializable
{
protected:
	std::vector<SAnimationKey> m_positionKeys;
	std::vector<SAnimationKey> m_quaternionKeys;
	std::vector<SAnimationKey> m_scaleKeys;
	std::set<float> m_timeTable;

public:
	MakeGetter(m_timeTable, TimeTable);

public:
	void AddPositionKey(const float& timeIn, const DirectX::XMVECTOR& positionIn);
	void AddQuaternionKey(const float& timeIn, const DirectX::XMVECTOR& quaternionIn);
	void AddScaleKey(const float& timeIn, const DirectX::XMVECTOR& scaleIn);

private:
	static void AddKey(
		std::vector<SAnimationKey>& keys,
		const float& timeIn,
		const DirectX::XMVECTOR& keyData
	);
	static DirectX::XMVECTOR GetLerpedVectorKey(
		const std::vector<SAnimationKey>& keys,
		const float& timeIn,
		const std::function<DirectX::XMVECTOR(const DirectX::XMVECTOR&, const DirectX::XMVECTOR&, const float&)>& interpolationMethod
	);

private:
	static std::function<DirectX::XMVECTOR(const DirectX::XMVECTOR&, const DirectX::XMVECTOR&, const float&)> lerpFunction;
	static std::function<DirectX::XMVECTOR(const DirectX::XMVECTOR&, const DirectX::XMVECTOR&, const float&)> slerpFunction;

public:
	DirectX::XMMATRIX GetLocalTransformation(const float& timeIn) const;

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class AnimationAsset : public AAsset
{
public:
	AnimationAsset() = default;
	AnimationAsset(const std::string& assetName);
	virtual ~AnimationAsset();

protected:
	float m_duration = 0.f;
	float m_ticksPerSecond = 0.f;

public:
	void SetAnimationDuration(const float& durationIn, const float& ticksPerSecond);
	
protected:
	std::unordered_map<std::string, AnimChannel> boneNameToAnimChannels;

public:
	MakeGetter(m_duration, Duration);
	MakeGetter(m_ticksPerSecond, TicksPerSecond);
	MakeGetter(boneNameToAnimChannels, BoneNameToAnimChannels);

public:
	void AddAnimChaannel(const std::string& boneName, const AnimChannel& animChannel);
	void AddAnimChannel(const std::string& boneName, AnimChannel&& animChannel);

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

class IAnimationProvider
{
public:
	virtual AnimationAsset* const GetAnimationAsset(
		const std::string& assetName
	) = 0;
};