#include "pch.h"
#include "AssetWriter.h"

using namespace std;

AAssetWriter::AAssetWriter(const string& assetSavePath)
	: m_assetSavePath(assetSavePath)
{
}

AAssetWriter::~AAssetWriter()
{
}

void AAssetWriter::SaveAssets(const EAssetType& assetType, vector<shared_ptr<AAsset>> assets)
{
    AssetWrapper assetWrapper;
    assetWrapper.SetAssetType(assetType);

    for (const shared_ptr<AAsset>& asset : assets)
    {
        FILE* fileIn = nullptr;
        const string assetPathStr = m_assetSavePath + asset->GetAssetName() + AAsset::AssetExtension;

        fopen_s(&fileIn, assetPathStr.c_str(), "wb");
        if (fileIn != nullptr)
        {
            assetWrapper.Serialize(fileIn);
            asset->Serialize(fileIn);
            fclose(fileIn);
        }
    }
}
