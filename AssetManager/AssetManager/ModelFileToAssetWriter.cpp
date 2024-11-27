#include "pch.h"
#include "ModelFileToAssetWriter.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "StringHelper.h"

#include "BoneAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "BaseTextureAsset.h"
#include "ModelMaterialAsset.h"

using namespace std;

vector<string> ModelFileToAssetWriter::ModelFileExtensions = { ".fbx", ".obj", ".gltf" };

ModelFileToAssetWriter::ModelFileToAssetWriter(const string& assetSavePath)
	: AAssetWriter(assetSavePath)
{
}

ModelFileToAssetWriter::~ModelFileToAssetWriter()
{
}

unordered_map<EAssetType, vector<shared_ptr<AAsset>>> ModelFileToAssetWriter::WriteToAssets(const std::string& filePath)
{
    unordered_map<EAssetType, vector<shared_ptr<AAsset>>> result;
    vector<unordered_map<EAssetType, vector<shared_ptr<AAsset>>>> writtenAssetsSet;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        StringHelper::ConvertACPToUTF8(filePath),
        aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_ConvertToLeftHanded
    );

    if (scene)
    {
        if (scene->HasMaterials())
        {
            writtenAssetsSet.emplace_back(LoadTexturesAndMaterials(scene));
        }

        // Load Mesh
        if (scene->HasMeshes())
        {
            //LoadMeshAssetFromFile(FileExtensionIn == ".gltf", FileNameIn, scene);
        }

        if (scene->HasAnimations())
        {
            //LoadAnimationAssetFromFile(FileNameIn, scene);
        }


        for (auto& writtenAssets : writtenAssetsSet)
        {
            for (auto& writtenAsset : writtenAssets)
            {
                const EAssetType& assetType = writtenAsset.first;
                const vector<shared_ptr<AAsset>>& loadedAssets = writtenAsset.second;

                result[assetType].insert(result[assetType].end(), loadedAssets.begin(), loadedAssets.end());
            }
        }
    }

	return result;
}

std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> ModelFileToAssetWriter::LoadTexturesAndMaterials(const aiScene* scene)
{
    return std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>>();
}
