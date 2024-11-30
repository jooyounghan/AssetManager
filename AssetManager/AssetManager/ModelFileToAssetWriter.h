#pragma once
#include "AssetWriter.h"
#include "SkeletalMeshAssetWriter.h"

struct aiScene;
struct aiMaterial;

enum aiTextureType;

class BaseTextureAsset;
class BoneAsset;
class AMeshAsset;

class ModelFileToAssetWriter : public AAssetWriter
{
public:
	ModelFileToAssetWriter(const std::string& assetSavePath);
	virtual ~ModelFileToAssetWriter();

protected:
	static std::string FbxExtension;
	static std::string ObjExtension;
	static std::string GltfExtension;

public:
	static std::vector<std::string> ModelFileExtensions;

private:
	StaticMeshAssetWriter m_staticMeshAssetWriter;
	SkeletalMeshAssetWriter m_skeletalMehsAssetWriter;

public:
	virtual std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> SaveAsAssets(const std::string& filePath) override;
	virtual bool IsAcceptableFilePath(const std::string& filePath) override;

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
	void LoadMeshes(
		const aiScene* const scene,
		const std::shared_ptr<AMeshAsset>& meshAsset,
		MeshAssetWriter& meshAssetWriter,
		const bool& isGltf
	);
	uint32_t GetLODLevelFromMeshName(const std::string&  meshName);

private:
	std::unordered_map<EAssetType, std::vector<std::shared_ptr<AAsset>>> LoadAnimations(
		const aiScene* const scene,
		const std::string& fileName
	);
};


