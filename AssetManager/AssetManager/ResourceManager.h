#pragma once
#include "BaseTextureAsset.h"

#include <windows.h>
#include <vector>

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
    ~ResourceManager();

protected:
    static BOOL CALLBACK EnumResouceNameProc(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName, LONG_PTR lParam);
    static SResourceInfo GetResourceInfo(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName);

private:
    HMODULE m_hModule;

private:
    std::vector<BaseTextureAsset*> m_loadedBaseTextureAssets;

public:
    std::vector<BaseTextureAsset*> LoadBitmapResources();
};

class IResourceProvider
{
public:
    virtual BaseTextureAsset GetResourceAsset(const std::string& assetName) = 0;
};
