#pragma once
#include "Asset.h"

#include <exception>
#include <string>

class AssetException : public std::exception 
{
private:
    const AAsset& m_assetCached;
    std::string m_message;

public:
    explicit AssetException(
        const AAsset& assetIn,
        const std::string& msg
    );

public:
    virtual const char* what() const noexcept override;
};
