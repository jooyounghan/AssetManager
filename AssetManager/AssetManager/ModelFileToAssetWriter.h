#pragma once
#include "AssetWriter.h"

struct aiScene;
struct aiMaterial;

enum aiTextureType;

class BaseTextureAsset;
class BoneAsset;

class ModelFileToAssetWriter : AAssetWriter
{
public:
	ModelFileToAssetWriter(const std::string& assetSavePath);
	virtual ~ModelFileToAssetWriter();

public:
	static std::string FbxExtension;
	static std::string ObjExtension;
	static std::string GltfExtension;
	static std::vector<std::string> ModelFileExtensions;

public:
	virtual std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> WriteToAssets(const std::string& filePath) override;

private:
	std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> LoadTexturesAndMaterials(const aiScene* const scene);
	std::shared_ptr<BaseTextureAsset> LoadBaseTextureFromMaterial(
		const aiScene* const scene,
		aiMaterial* material,
		aiTextureType textureType
	);
		
private:
	std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> LoadMeshesAndBones(
		const aiScene* const scene, 
		const std::string& fileName,
		const bool& isGltf
	);
	bool HasBones(const aiScene* const scene);
	void LoadBones(
		const aiScene* const scene, 
		const std::shared_ptr<BoneAsset>& boneAsset
	);
};

