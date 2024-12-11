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
	virtual std::unordered_map<EAssetType, std::vector<AAsset*>> SaveAsAssets(const std::string& filePath) override;
	virtual bool IsAcceptableFilePath(const std::string& filePath) const override;

private:
	std::unordered_map<EAssetType, std::vector<AAsset*>> LoadTexturesAndMaterials(const aiScene* const scene) const;
	BaseTextureAsset* LoadBaseTextureFromMaterial(
		const aiScene* const scene,
		aiMaterial* material,
		aiTextureType textureType
	) const;
		
private:
	std::unordered_map<EAssetType, std::vector<AAsset*>> LoadMeshesAndBones(
		const aiScene* const scene, 
		const std::string& fileName,
		const bool& isGltf
	) ;
	bool HasBones(const aiScene* const scene);
	void LoadBones(
		const aiScene* const scene, 
		BoneAsset* const boneAsset
	) const;
	void LoadMeshes(
		const aiScene* const scene,
		AMeshAsset* const meshAsset,
		MeshAssetWriter& meshAssetWriter,
		const bool& isGltf
	) const;
	uint32_t GetLODLevelFromMeshName(const std::string&  meshName) const;

private:
	std::unordered_map<EAssetType, std::vector<AAsset*>> LoadAnimations(
		const aiScene* const scene,
		const std::string& fileName
	);
};


