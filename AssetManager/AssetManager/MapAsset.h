#pragma once
#include "Asset.h"

class MapAsset : public AAsset
{
public:
	MapAsset(const std::string& assetNameIn);
	virtual ~MapAsset();

protected:
	std::string m_spe
};

