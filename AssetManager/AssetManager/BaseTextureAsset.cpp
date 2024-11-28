#include "pch.h"
#include "BaseTextureAsset.h"

using namespace std;

BaseTextureAsset::BaseTextureAsset(
	const string& assetName,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	uint8_t* imageBufferIn
)
	: TextureAsset(assetName, widthIn, heightIn, 1)
{
	m_originalSizePerArray.push_back(m_width * m_height * 4);

	vector<uint8_t> originalBuffer;
	originalBuffer.resize(m_width * m_height * 4);
	memcpy(originalBuffer.data(), imageBufferIn, originalBuffer.size());

	vector<vector<uint8_t>> originalBufferPerArray;
	originalBufferPerArray.push_back(move(originalBuffer));

	m_compressedBufferPerArray = CompressDataArray(originalBufferPerArray);
}

BaseTextureAsset::~BaseTextureAsset()
{

}