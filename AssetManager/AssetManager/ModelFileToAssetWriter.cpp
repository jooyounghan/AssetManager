#include "pch.h"
#include "ModelFileToAssetWriter.h"

#include "StringHelper.h"

#include "BoneAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "BaseTextureAsset.h"
#include "ModelMaterialAsset.h"
#include "AnimationAsset.h"

#include <filesystem>
#include <regex>

using namespace std;
using namespace std::filesystem;
using namespace DirectX;

string ModelFileToAssetWriter::FbxExtension = ".fbx";
string ModelFileToAssetWriter::ObjExtension = ".obj";
string ModelFileToAssetWriter::GltfExtension = ".gltf";
vector<string> ModelFileToAssetWriter::ModelFileExtensions = { FbxExtension, ObjExtension, GltfExtension };

ModelFileToAssetWriter::ModelFileToAssetWriter(const string& assetSavePath)
	: AAssetWriter(assetSavePath)
{
}

ModelFileToAssetWriter::~ModelFileToAssetWriter()
{
}

unordered_map<EAssetType, vector<shared_ptr<AAsset>>> ModelFileToAssetWriter::SaveAsAssets(const string& filePath)
{
    unordered_map<EAssetType, vector<shared_ptr<AAsset>>> result;
    vector<unordered_map<EAssetType, vector<shared_ptr<AAsset>>>> writtenAssetsSet;

    const string& fileName = path(filePath).stem().string();
    const string& extension = path(filePath).extension().string();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        StringHelper::ConvertACPToUTF8(m_assetSavePath + filePath),
        aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords | aiProcess_ConvertToLeftHanded
    );



    if (scene != nullptr)
    {
        if (scene->HasMaterials())
        {
            writtenAssetsSet.emplace_back(LoadTexturesAndMaterials(scene));
        }

        // Load Mesh
        if (scene->HasMeshes())
        {
            writtenAssetsSet.emplace_back(LoadMeshesAndBones(scene, fileName, extension == GltfExtension));
        }

        if (scene->HasAnimations())
        {
            writtenAssetsSet.emplace_back(LoadAnimations(scene, fileName));
        }
    }

    for (auto& writtenAssets : writtenAssetsSet)
    {
        for (auto& writtenAsset : writtenAssets)
        {
            const EAssetType& assetType = writtenAsset.first;
            const vector<shared_ptr<AAsset>>& assets = writtenAsset.second;

            SaveAssets(assetType, assets);

            result[assetType].insert(result[assetType].end(), assets.begin(), assets.end());
        }
    }

    return result;
}

bool ModelFileToAssetWriter::IsAcceptableFilePath(const string& filePath)
{
    const string& extension = path(filePath).extension().string();
    return (find(ModelFileExtensions.begin(), ModelFileExtensions.end(), extension) != ModelFileExtensions.end());
}

unordered_map<EAssetType, vector<shared_ptr<AAsset>>> ModelFileToAssetWriter::LoadTexturesAndMaterials(const aiScene* const scene)
{
    unordered_map<EAssetType, vector<shared_ptr<AAsset>>> result;

    for (size_t material_idx = 0; material_idx < scene->mNumMaterials; ++material_idx)
    {
        aiMaterial* material = scene->mMaterials[material_idx];

        shared_ptr<ModelMaterialAsset> modelMaterialAsset = make_shared<ModelMaterialAsset>(material->GetName().C_Str());

        if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0)
        {
            shared_ptr<BaseTextureAsset> ambientOcculusion = LoadBaseTextureFromMaterial(scene, material, aiTextureType_AMBIENT_OCCLUSION);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_AMBIENTOCCULUSION, ambientOcculusion);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(ambientOcculusion);
        }
        if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            shared_ptr<BaseTextureAsset> specular = LoadBaseTextureFromMaterial(scene, material, aiTextureType_SPECULAR);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_SPECULAR, specular);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(specular);
        }
        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            shared_ptr<BaseTextureAsset> diffuse = LoadBaseTextureFromMaterial(scene, material, aiTextureType_DIFFUSE);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_DIFFUSE, diffuse);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(diffuse);
        }
        if (material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS) > 0)
        {
            shared_ptr<BaseTextureAsset> roughness = LoadBaseTextureFromMaterial(scene, material, aiTextureType_DIFFUSE_ROUGHNESS);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_ROUGHNESS, roughness);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(roughness);
        }
        if (material->GetTextureCount(aiTextureType_METALNESS) > 0)
        {
            shared_ptr<BaseTextureAsset> metalness = LoadBaseTextureFromMaterial(scene, material, aiTextureType_METALNESS);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_METALIC, metalness);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(metalness);
        }
        if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
        {
            shared_ptr<BaseTextureAsset> normal = LoadBaseTextureFromMaterial(scene, material, aiTextureType_NORMALS);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_NORMAL, normal);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(normal);
        }
        if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
        {
            shared_ptr<BaseTextureAsset> height = LoadBaseTextureFromMaterial(scene, material, aiTextureType_HEIGHT);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_HEIGHT, height);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(height);
        }
        if (material->GetTextureCount(aiTextureType_EMISSIVE) > 0)
        {
            shared_ptr<BaseTextureAsset> emissive = LoadBaseTextureFromMaterial(scene, material, aiTextureType_EMISSIVE);
            modelMaterialAsset->SetModelMaterialTexture(EModelMaterialTexture::MODEL_MATERIAL_TEXTURE_EMISSIVE, emissive);
            result[EAssetType::ASSET_TYPE_BASE_TEXTURE].emplace_back(emissive);
        }
        result[EAssetType::ASSET_TYPE_MODEL_MATERIAL].emplace_back(modelMaterialAsset);
    }

    return result;
}

shared_ptr<BaseTextureAsset> ModelFileToAssetWriter::LoadBaseTextureFromMaterial(const aiScene* const scene, aiMaterial* material, aiTextureType textureType)
{
    shared_ptr<BaseTextureAsset> asset;
    aiString aiTexturePath;
    if (material->GetTexture(textureType, 0, &aiTexturePath) == aiReturn_SUCCESS)
    {
        const string texturePath = aiTexturePath.C_Str();
        const string textureName = path(texturePath).stem().string();

        const aiTexture* texture = scene->GetEmbeddedTexture(texturePath.c_str());
        if (texture != nullptr)
        {
            int widthOut, heightOut, channelOut;
            stbi_uc* imageBuffer = stbi_load_from_memory((const stbi_uc*)texture->pcData, texture->mWidth, &widthOut, &heightOut, &channelOut, 4);
            if (imageBuffer != nullptr)
            {
                asset = make_shared<BaseTextureAsset>(textureName, widthOut, heightOut, imageBuffer);
                stbi_image_free(imageBuffer);
            }
        }
    }
    return asset;
}

unordered_map<EAssetType, vector<shared_ptr<AAsset>>> ModelFileToAssetWriter::LoadMeshesAndBones(
    const aiScene* const scene,
    const string& fileName,
    const bool& isGltf
)
{
    unordered_map<EAssetType, vector<shared_ptr<AAsset>>> result;

    XMMATRIX rootTransform = DirectX::XMMatrixIdentity();
    aiNode* rootNode = scene->mRootNode;

    if (HasBones(scene))
    {
        shared_ptr<SkeletalMeshAsset> skeletalMeshAsset = make_shared<SkeletalMeshAsset>(fileName + "_Skeletal");
        result[EAssetType::ASSET_TYPE_SKELETAL].emplace_back(skeletalMeshAsset);

        shared_ptr<BoneAsset> boneAsset = make_shared<BoneAsset>(fileName + "_Bone");
        result[EAssetType::ASSET_TYPE_BONE].emplace_back(boneAsset);
        skeletalMeshAsset->SetBoneAsset(boneAsset);
        
        LoadBones(scene, boneAsset);
        LoadMeshes(scene, skeletalMeshAsset, m_skeletalMehsAssetWriter, isGltf);
    }
    else
    {
        shared_ptr<StaticMeshAsset> staticMeshAsset = make_shared<StaticMeshAsset>(fileName + "_Static");
        result[EAssetType::ASSET_TYPE_STATIC].emplace_back(staticMeshAsset);

        LoadMeshes(scene, staticMeshAsset, m_staticMeshAssetWriter, isGltf);
    }

    return result;
}

 bool ModelFileToAssetWriter::HasBones(const aiScene* const scene)
{
    for (uint32_t idx = 0; idx < scene->mNumMeshes; ++idx)
    {
        const aiMesh* mesh = scene->mMeshes[idx];
        if (mesh->mNumBones > 0)
        {
            return true;
        }
    }
    return false;
}

 void ModelFileToAssetWriter::LoadBones(
     const aiScene* const scene, 
     const shared_ptr<BoneAsset>& boneAsset
 )
 {
     unordered_map<string, shared_ptr<Bone>> nodeNameToBone;

     for (uint32_t meshIdx = 0; meshIdx < scene->mNumMeshes; ++meshIdx)
     {
         aiMesh* currentMesh = scene->mMeshes[meshIdx];
         if (currentMesh->HasBones())
         {
             for (uint32_t boneIdx = 0; boneIdx < currentMesh->mNumBones; ++boneIdx)
             {
                 shared_ptr<Bone> bone = make_shared<Bone>();

                 XMFLOAT4X4 offsetMatrix;
                 aiBone* currentBone = currentMesh->mBones[boneIdx];

                 const string boneName = currentBone->mName.C_Str();
                 memcpy(&offsetMatrix, &currentBone->mOffsetMatrix, sizeof(XMFLOAT4X4));
                 bone->SetBoneProperties(boneIdx, XMMatrixTranspose(DirectX::XMLoadFloat4x4(&offsetMatrix)));

                 boneAsset->AddBone(bone, boneName);
                 nodeNameToBone.emplace(boneName, bone);
             }
             break;
         }
     }

     function<void(shared_ptr<Bone>, aiNode*)> dfs = [&](shared_ptr<Bone> parentBone, aiNode* node) {

         shared_ptr<Bone> newParentBone = parentBone;

         const string& nodeName = node->mName.C_Str();
         if (nodeNameToBone.find(nodeName) != nodeNameToBone.end())
         {
             shared_ptr<Bone> currentBone = nodeNameToBone[nodeName];
             if (parentBone != nullptr)
             {
                 parentBone->AddChildBone(currentBone);
             }
             else
             {
                 boneAsset->SetRootBone(currentBone);
             }
             currentBone->SetParentBone(parentBone);
             newParentBone = currentBone;
         }

         for (uint32_t idx = 0; idx < node->mNumChildren; ++idx)
         {
             aiNode* childNode = node->mChildren[idx];
             dfs(newParentBone, childNode);
         }
     };

     dfs(nullptr, scene->mRootNode);
 }

 void ModelFileToAssetWriter::LoadMeshes(
     const aiScene* const scene, 
     const shared_ptr<AMeshAsset>& meshAsset,
     MeshAssetWriter& meshAssetWriter,
     const bool& isGltf
 )
 {
     function<void(shared_ptr<AMeshAsset>, aiNode*, const XMMATRIX& tranformation)> dfs =
         [&](shared_ptr<AMeshAsset> meshAsset, aiNode* node, const XMMATRIX& tranformation)
         {
             XMMATRIX currentTransformation(&node->mTransformation.a1);

             currentTransformation = XMMatrixTranspose(currentTransformation) * tranformation;

             for (uint32_t meshIdx = 0; meshIdx < node->mNumMeshes; ++meshIdx)
             {
                 const aiMesh* const mesh = scene->mMeshes[node->mMeshes[meshIdx]];
                 const string& meshName = mesh->mName.C_Str();

                 const uint32_t lodLevel = GetLODLevelFromMeshName(meshName);
                 shared_ptr<MeshPartsData> meshPartsData = meshAsset->GetMeshPartData(lodLevel);
                 meshAssetWriter.LoadMeshPartData(meshPartsData, isGltf, mesh, currentTransformation);
             }

             for (uint32_t idx = 0; idx < node->mNumChildren; ++idx)
             {
                 dfs(meshAsset, node->mChildren[idx], currentTransformation);
             }
         };

     dfs(meshAsset, scene->mRootNode, XMMatrixIdentity());
 }

 inline uint32_t ModelFileToAssetWriter::GetLODLevelFromMeshName(const string& meshName)
 {
     uint32_t lodlevel = 0;
     regex lodMeshNamePattern("\\.(\\d+)");
     smatch match;

     if (regex_search(meshName, match, lodMeshNamePattern))
     {
         string lod_number = match[1];
         lodlevel = max(lodlevel, static_cast<uint32_t>(stoul(lod_number)));
     }

     return lodlevel;
 }

 unordered_map<EAssetType, vector<shared_ptr<AAsset>>> ModelFileToAssetWriter::LoadAnimations(
     const aiScene* const scene,
     const string& fileName
 )
 {
     unordered_map<EAssetType, vector<shared_ptr<AAsset>>> result;

     for (uint32_t animationIdx = 0; animationIdx < scene->mNumAnimations; ++animationIdx)
     {
         shared_ptr<AnimationAsset> animationAsset;
         result[EAssetType::ASSET_TYPE_ANIMATION].emplace_back(animationAsset);
         
         aiAnimation* animation = scene->mAnimations[animationIdx];
         animationAsset->SetAnimationDuration(
             static_cast<float>(animation->mDuration), 
             static_cast<float>(animation->mTicksPerSecond)
         );

         for (uint32_t channelIdx = 0; channelIdx < animation->mNumChannels; ++channelIdx)
         {
             AnimChannel animChannel;

             aiNodeAnim* channel = animation->mChannels[channelIdx];
             const string channelName = channel->mNodeName.C_Str();

             for (uint32_t positionIdx = 0; positionIdx < channel->mNumPositionKeys; ++positionIdx)
             { 
                 const aiVectorKey& positionKey = channel->mPositionKeys[positionIdx];
                 animChannel.AddPositionKey(
                     static_cast<float>(positionKey.mTime), 
                     XMVectorSet(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z, 1.f)
                 );
             }

             for (uint32_t quaternionIdx = 0; quaternionIdx < channel->mNumRotationKeys; ++quaternionIdx)
             {
                 const aiQuatKey& quaternionKey = channel->mRotationKeys[quaternionIdx];
                 animChannel.AddQuaternionKey(
                     static_cast<float>(quaternionKey.mTime),
                     XMVectorSet(quaternionKey.mValue.x, quaternionKey.mValue.y, quaternionKey.mValue.z, quaternionKey.mValue.w)
                 );
             }

             for (uint32_t scaleIdx = 0; scaleIdx < channel->mNumScalingKeys; ++scaleIdx)
             {
                 const aiVectorKey& scaleKey = channel->mScalingKeys[scaleIdx];
                 animChannel.AddScaleKey(
                     static_cast<float>(scaleKey.mTime),
                     XMVectorSet(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z, 0.f)
                 );
             }

             animationAsset->AddAnimChannel(channelName, move(animChannel));
         }
     }
     return result;
 }
