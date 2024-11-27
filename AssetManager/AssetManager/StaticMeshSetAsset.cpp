#include "pch.h"
#include "StaticMeshSetAsset.h"

using namespace std;

StaticMeshSetAsset::StaticMeshSetAsset(const std::string& assetName)
	: MeshSetAsset(assetName)
{
}

StaticMeshSetAsset::~StaticMeshSetAsset()
{
}

void StaticMeshSetAsset::UpdateStaticMeshAndMaterialAsset(
	IStaticMeshProvider& staticMeshProvider, 
	IModelMaterialProvider& modelMaterialProvider
)
{
	m_staticMeshAssets.clear();
	m_modelMaterialAssets.clear();

	for (auto& staticMeshAssetNamesToModelMaterialName : m_meshAssetNamesToModelMaterialNames)
	{
		const string& staticMeshAssetName = staticMeshAssetNamesToModelMaterialName.first;
		const string& modelMaterialAssetName = staticMeshAssetNamesToModelMaterialName.second;

		m_staticMeshAssets.emplace_back(staticMeshProvider.GetStaticMeshAsset(staticMeshAssetName));
		m_modelMaterialAssets.emplace_back(modelMaterialProvider.GetModelMaterialAsset(modelMaterialAssetName));
	}
}