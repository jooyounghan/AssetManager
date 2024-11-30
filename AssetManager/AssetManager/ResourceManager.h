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

BOOL CALLBACK EnumResouceNameProc(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName, LONG_PTR lParam);
SResourceInfo GetResourceInfo(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName);

class ResourceManager
{
public:
    ResourceManager(HMODULE hModule = nullptr);

private:
    HMODULE m_hModule;

public:
    std::vector<std::shared_ptr<BaseTextureAsset>> LoadBitmapResources() const;
};

