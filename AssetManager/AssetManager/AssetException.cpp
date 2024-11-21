#include "pch.h"
#include "AssetException.h"

AssetException::AssetException(
    const AAsset& assetIn,
    const std::string& msg
)
    : m_assetCached(assetIn), m_message(msg)
{

}

const char* AssetException::what() const noexcept
{
    return m_message.c_str();
}
