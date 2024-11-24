#pragma once
#include "SerializeHelper.h"
#include "HeaderHelper.h"

class AAsset : public ISerializable
{
public:
	AAsset(const std::string& assetPathIn, const std::string& assetNameIn);
	virtual ~AAsset();

protected:
	std::string m_assetPath;
	std::string m_assetName;
	bool m_isModified = false;
	MakeGetter(m_isModified, IsModified);

public:
	const std::string& GetAssetPath() { return m_assetPath; }
	const std::string& GetAssetName() { return m_assetName; }

public:
	virtual void Serialize(FILE* fileIn) const override;
	virtual void Deserialize(FILE* fileIn) override;
};

