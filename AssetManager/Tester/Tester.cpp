#include <iostream>
#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"
#include "ModelMaterialAsset.h"
#include "MeshAsset.h"
#include "BoneAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "AnimationAsset.h"
#include "IBLMaterialAsset.h"
#include "MapAsset.h"
#include "AssetReader.h"
#include "AssetManager.h"

using namespace std;

int main()
{
	AssetManager a;
	a.PreloadAsset();


	AssetReader assetReader("./Assets/");
	assetReader.UpdatePreloadArgs();
	auto test100 =assetReader.GetLoadedAsset();



	MapAsset test11("Test");
	test11.Serialize(nullptr);
	test11.Deserialize(nullptr);

	IBLMaterialAsset test10("Test");
	test10.Serialize(nullptr);
	test10.Deserialize(nullptr);

	AnimationAsset test9("Test");
	test9.Serialize(nullptr);
	test9.Deserialize(nullptr);

	SAnimationKey test8{ 4.f, DirectX::XMVectorSet(1.f, 2.f, 3.f, 4.f) };

	SkeletalMeshAsset test7("Test");
	test7.Serialize(nullptr);
	test7.Deserialize(nullptr);

	StaticMeshAsset test6("Test");
	test6.Serialize(nullptr);
	test6.Deserialize(nullptr);

	BoneAsset test5("Test");
	test5.Serialize(nullptr);
	test5.Deserialize(nullptr);

	ModelMaterialAsset test3("Test");

	test3.Serialize(nullptr);
	test3.Deserialize(nullptr);

	ScratchTextureAsset test2;
	test2.CompressDataArray(vector<vector<uint8_t>>());
	test2.DecompressDataArray();
	test2.Serialize(nullptr);
	test2.Deserialize(nullptr);

	BaseTextureAsset test("Test", 100, 100, nullptr);
	test.CompressDataArray(vector<vector<uint8_t>>());
	test.DecompressDataArray();
	test.Serialize(nullptr);
	test.Deserialize(nullptr);




	bool t = true;
}
