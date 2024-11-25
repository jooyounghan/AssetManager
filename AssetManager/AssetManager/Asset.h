#pragma once
#include "SerializeHelper.h"
#include "HeaderHelper.h"

class AAsset : public ISerializable
{
public:
	AAsset(const std::string& assetNameIn);
	virtual ~AAsset();

protected:
	std::string m_assetName;
	bool m_isModified = false;
	MakeGetter(m_assetName, AssetName);
	MakeGetter(m_isModified, IsModified);

public:
	const std::string& GetAssetName() { return m_assetName; }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;

public:
	void SerializeAssetName(
		const std::shared_ptr<AAsset>& asset,
		FILE* fileIn
	) const;
};

