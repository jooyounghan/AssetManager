#pragma once
#include "BaseTextureAsset.h"

#include <windows.h>
#include <vector>
#include <memory>

struct SResourceSearchContext 
{
    int resourceId;
    std::string resourceName;
};

struct SResourceInfo 
{
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> resourceData;
};

class ResourceManager
{
public:
    ResourceManager(HMODULE hModule = nullptr);

protected:
    static BOOL CALLBACK EnumResouceNameProc(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName, LONG_PTR lParam);
    static SResourceInfo GetResourceInfo(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName);

private:
    HMODULE m_hModule;

public:
    std::vector<std::shared_ptr<BaseTextureAsset>> LoadBitmapResources() const;
};

class IResourceProvider
{
public:
    virtual std::shared_ptr<BaseTextureAsset> GetResourceAsset(
        const std::string& assetName
    ) = 0;
};
