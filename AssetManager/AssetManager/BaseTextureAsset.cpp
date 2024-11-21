#include "pch.h"
#include "BaseTextureAsset.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

using namespace std;

BaseTextureAsset::BaseTextureAsset(
	const string& assetPathIn, 
	const string& assetNameIn
)
	: ATextureAsset(assetPathIn, assetNameIn, 0, 0, 0)
{
}

BaseTextureAsset::BaseTextureAsset(
	const string& assetPathIn, 
	const string& assetNameIn,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	uint8_t* imageBufferIn
)
	: ATextureAsset(assetPathIn, assetNameIn, widthIn, heightIn, 1)
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

vector<vector<uint8_t>> BaseTextureAsset::CompressDataArray(
	const vector<vector<uint8_t>>& originalBufferPerArray
)
{
	vector<vector<uint8_t>> result;

	m_compressedSizePerArray.resize(m_arraySize);

	for (size_t arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{
		const uint8_t* originalBufferIn = originalBufferPerArray[arrayIdx].data();
		size_t& compressedSize = m_compressedSizePerArray[arrayIdx];

		uint8_t* compressedMemoryBuffer = stbi_write_png_to_mem(originalBufferIn, m_width * 4, m_width, m_height, 4, (int*)&compressedSize);
		vector<uint8_t> compressedData(compressedMemoryBuffer, compressedMemoryBuffer + compressedSize);
		stbi_image_free(compressedMemoryBuffer);
		result.push_back(move(compressedData));
	}
	return result;
}

vector<vector<uint8_t>> BaseTextureAsset::DecompressDataArray()
{
	vector<vector<uint8_t>> result;
	for (size_t arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{
		int width, height, channel;

		const uint8_t* compressedBufferIn = m_compressedBufferPerArray[arrayIdx].data();
		const int compressedSize = static_cast<int>(m_compressedSizePerArray[arrayIdx]);
		const size_t& originalSize = m_originalSizePerArray[arrayIdx];

		uint8_t* originalMemoryBuffer = stbi_load_from_memory(compressedBufferIn, compressedSize, &width, &height, &channel, 4);
		vector<uint8_t> originalData(originalMemoryBuffer, originalMemoryBuffer + originalSize);
		stbi_image_free(originalMemoryBuffer);
		result.push_back(move(originalData));
	}
	return result;
}
