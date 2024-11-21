#include <iostream>
#include "BaseTextureAsset.h"
#include "ScratchTextureAsset.h"
#include "MaterialAsset.h"
#include "MeshAsset.h"
#include "BoneAsset.h"

using namespace std;

int main()
{
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
