#include "pch.h"
#include "AnimationAsset.h"

using namespace std;
using namespace DirectX;

constexpr float AnimationTimeErr = 1E-3f;

std::function<XMVECTOR(const XMVECTOR&, const XMVECTOR&, const float&)> AnimChannel::lerpFunction 
	= [](const XMVECTOR& A, const XMVECTOR& B, const float& C) -> XMVECTOR {
		return XMVectorLerp(A, B, C);
	};

std::function<XMVECTOR(const XMVECTOR&, const XMVECTOR&, const float&)> AnimChannel::slerpFunction
	= [](const XMVECTOR& A, const XMVECTOR& B, const float& C) -> XMVECTOR {
			return XMQuaternionSlerp(A, B, C);
	};

void AnimChannel::AddPositionKey(const float& timeIn, const XMVECTOR& positionIn)
{
	AddKey(m_positionKeys, timeIn, positionIn);
}

void AnimChannel::AddQuaternionKey(const float& timeIn, const XMVECTOR& quaternionIn)
{
	AddKey(m_quaternionKeys, timeIn, quaternionIn);
}

void AnimChannel::AddScaleKey(const float& timeIn, const XMVECTOR& scaleIn)
{
	AddKey(m_scaleKeys, timeIn, scaleIn);
}

void AnimChannel::AddKey(
	std::vector<SAnimationKey>& keys, 
	const float& timeIn, 
	const DirectX::XMVECTOR& keyData
)
{
	SAnimationKey animationKey{ timeIn, keyData };
	keys.emplace_back(move(animationKey));
}

DirectX::XMVECTOR AnimChannel::GetLerpedVectorKey(
	const std::vector<SAnimationKey>& keys,
	const float& timeIn,
	const std::function<XMVECTOR(const XMVECTOR&, const XMVECTOR&, const float&)>& interpolationMethod
)
{
	if (keys.empty())
	{
		return XMVECTOR();
	}

	for (size_t idx = 0; idx < keys.size() - 1; ++idx)
	{
		const SAnimationKey& keyFrom = keys[idx];
		const SAnimationKey& keyTo = keys[idx + 1];

		if (keyFrom.time - AnimationTimeErr < timeIn && timeIn < keyTo.time + AnimationTimeErr)
		{
			return interpolationMethod(keyFrom.animationData, keyTo.animationData, (timeIn - keyFrom.time) / (keyTo.time - keyFrom.time));
		}
	}

	return keys[0].animationData;
}

DirectX::XMMATRIX AnimChannel::GetLocalTransformation(const float& timeIn)
{
	return XMMatrixAffineTransformation(
		GetLerpedVectorKey(m_scaleKeys, timeIn, lerpFunction),
		XMVectorZero(),
		GetLerpedVectorKey(m_quaternionKeys, timeIn, slerpFunction),
		GetLerpedVectorKey(m_positionKeys, timeIn, lerpFunction)
	);
}


void AnimChannel::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeSequenceContainer(m_positionKeys, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_quaternionKeys, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_scaleKeys, fileIn);
}

void AnimChannel::Deserialize(FILE* fileIn)
{
	m_positionKeys = DeserializeHelper::DeserializeSequenceContainer<vector<SAnimationKey>>(fileIn);
	m_quaternionKeys = DeserializeHelper::DeserializeSequenceContainer<vector<SAnimationKey>>(fileIn);
	m_scaleKeys = DeserializeHelper::DeserializeSequenceContainer<vector<SAnimationKey>>(fileIn);
}

AnimationAsset::AnimationAsset(const std::string& assetPathIn, const std::string& assetNameIn)
	: AAsset(assetPathIn, assetNameIn)
{
}

AnimationAsset::~AnimationAsset()
{
}

void AnimationAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeElement(m_duration, fileIn);
	SerializeHelper::SerializeElement(m_ticksPerSecond, fileIn);

	SerializeHelper::SerializeContainerSize(boneNameToAnimChannels, fileIn);
	for (auto& boneNameToAnimChannel : boneNameToAnimChannels)
	{
		const string& boneName = boneNameToAnimChannel.first;
		const AnimChannel& animChannel = boneNameToAnimChannel.second;

		SerializeHelper::SerializeString(boneName, fileIn);
		animChannel.Serialize(fileIn);
	}
}

void AnimationAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);
	m_duration = DeserializeHelper::DeserializeElement<float>(fileIn);
	m_ticksPerSecond = DeserializeHelper::DeserializeElement<float>(fileIn);

	size_t containerSize = DeserializeHelper::DeserializeElement<size_t>(fileIn);
	for (size_t containerIdx = 0; containerIdx < containerSize; ++containerIdx)
	{
		string boneName = DeserializeHelper::DeserializeString(fileIn);
		AnimChannel animChannel;
		animChannel.Deserialize(fileIn);
		boneNameToAnimChannels.emplace(boneName, animChannel);
	}
}
