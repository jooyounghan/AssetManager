#pragma once
#include "SerializeHelper.h"

class AAsset : public ISerializable
{
public:
	AAsset(const std::string& assetPathIn, const std::string& assetNameIn);
	virtual ~AAsset();

protected:
	std::string m_assetPath;
	std::string m_assetName;
	bool m_isModified = false;

public:
	const std::string& GetAssetPath() { return m_assetPath; }
	const std::string& GetAssetName() { return m_assetName; }
	const bool& IsModified() { return m_isModified; }

public:
	virtual void Serialize(FILE* fileIn) override;
	virtual void Deserialize(FILE* fileIn) override;
};

