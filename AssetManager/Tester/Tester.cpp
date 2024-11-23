#include <iostream>
#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"
#include "MaterialAsset.h"
#include "MeshAsset.h"
#include "BoneAsset.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"
#include "AnimationAsset.h"


using namespace std;

int main()
{
	AnimationAsset test9("./Test", "Test");
	test9.Serialize(nullptr);
	test9.Deserialize(nullptr);

	SAnimationKey test8{ 4.f, DirectX::XMVectorSet(1.f, 2.f, 3.f, 4.f) };

	SkeletalMeshAsset test7("./Test", "Test");
	test7.Serialize(nullptr);
	test7.Deserialize(nullptr);

	StaticMeshAsset test6("./Test", "Test");
	test6.Serialize(nullptr);
	test6.Deserialize(nullptr);

	BoneAsset test5("./Test", "Test");
	test5.Serialize(nullptr);
	test5.Deserialize(nullptr);

	MeshAsset test4("./Test", "Test");
	test4.Serialize(nullptr);
	test4.Deserialize(nullptr);

	MaterialAsset test3("./Test", "Test");

	test3.Serialize(nullptr);
	test3.Deserialize(nullptr);

	ScratchTextureAsset test2("./Test", "Test");
	test2.CompressDataArray(vector<vector<uint8_t>>());
	test2.DecompressDataArray();
	test2.Serialize(nullptr);
	test2.Deserialize(nullptr);

	BaseTextureAsset test("./Test", "Test", 100, 100, nullptr);
	test.CompressDataArray(vector<vector<uint8_t>>());
	test.DecompressDataArray();
	test.Serialize(nullptr);
	test.Deserialize(nullptr);




	bool t = true;
}
