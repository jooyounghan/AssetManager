#include "pch.h"
#include "ResourceManager.h"
#include "StringHelper.h"

#include <format>

using namespace std;

BOOL EnumResouceNameProc(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName, LONG_PTR lParam)
{
    vector<shared_ptr<BaseTextureAsset>>* result = reinterpret_cast<vector<shared_ptr<BaseTextureAsset>>*>(lParam);

    SResourceInfo resourceInfo = GetResourceInfo(hModule, lpszType, lpszName);
    if (IS_INTRESOURCE(lpszName))
    {
        const size_t resouceId = reinterpret_cast<size_t>(lpszName);
        result->emplace_back(make_shared<BaseTextureAsset>(
            format("Resouce_{}", to_string(resouceId)).c_str(),
            resourceInfo.width, resourceInfo.height / 2,
            resourceInfo.resourceData.data()
        ));
    }
    else 
    {
        result->emplace_back(make_shared<BaseTextureAsset>(
            StringHelper::ConvertWStringToACP(lpszName), resourceInfo.width, resourceInfo.height / 2,
            resourceInfo.resourceData.data()
        ));
    }
    return TRUE;
}


SResourceInfo GetResourceInfo(HMODULE hModule, LPCWSTR lpszType, LPWSTR lpszName)
{
    SResourceInfo resourceInfo = { 0, 0, {} };

    HRSRC hResource = FindResource(hModule, lpszName, lpszType);
    if (!hResource) return resourceInfo;

    HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
    if (!hLoadedResource) return resourceInfo;

    void* pResourceData = LockResource(hLoadedResource);
    if (!pResourceData) return resourceInfo;

    DWORD resourceSize = SizeofResource(hModule, hResource);
    if (resourceSize == 0) return resourceInfo;

    BITMAPINFOHEADER* pBitmapInfoHeader = reinterpret_cast<BITMAPINFOHEADER*>(pResourceData);
    resourceInfo.width = pBitmapInfoHeader->biWidth;
    resourceInfo.height = pBitmapInfoHeader->biHeight;

    int width, height, channel;
    stbi_load_from_memory((stbi_uc*)pResourceData, resourceInfo.width, &width, &height, &channel, 4);

    uint8_t* pBitmapPixels = reinterpret_cast<uint8_t*>(pBitmapInfoHeader) + pBitmapInfoHeader->biSize;
    uint32_t pixelDataSize = resourceSize - pBitmapInfoHeader->biSize;

    resourceInfo.resourceData.resize(resourceInfo.width * resourceInfo.height * 4);

    for (uint32_t i = 0, j = 0; i < pixelDataSize; i += 3, j += 4)
    {
        resourceInfo.resourceData[j] = pBitmapPixels[i + 2];
        resourceInfo.resourceData[j + 1] = pBitmapPixels[i + 1];
        resourceInfo.resourceData[j + 2] = pBitmapPixels[i];
        resourceInfo.resourceData[j + 3] = 255;
    }

    return resourceInfo;
}

ResourceManager::ResourceManager(HMODULE hModule)
    : m_hModule(hModule ? hModule : GetModuleHandle(nullptr)) 
{
}

std::vector<shared_ptr<BaseTextureAsset>> ResourceManager::LoadBitmapResources() const
{
    vector<shared_ptr<BaseTextureAsset>> result;

    if (!EnumResourceNames(m_hModule, RT_BITMAP, EnumResouceNameProc, reinterpret_cast<LONG_PTR>(&result)))
    {
    }

    return result;
}