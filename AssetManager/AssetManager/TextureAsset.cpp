#include "pch.h"
#include "TextureAsset.h"

using namespace std;

TextureAsset::TextureAsset(
	const string& assetName,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	const unsigned int& arraysizeIn
) 
	: AAsset(assetName), m_width(widthIn), m_height(heightIn), m_arraySize(arraysizeIn)
{

}

TextureAsset::~TextureAsset() 
{

}


vector<vector<uint8_t>> TextureAsset::CompressDataArray(
	const vector<vector<uint8_t>>& originalBufferPerArray
)
{
	vector<vector<uint8_t>> result;
	m_compressedSizePerArray.resize(m_arraySize);
	for (unsigned int arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{

		uLong compressedSize = compressBound(static_cast<uLong>(m_originalSizePerArray[arrayIdx]));
		vector<uint8_t> compressedData(compressedSize);

		int compressResult = compress2(
			compressedData.data(), &compressedSize,
			originalBufferPerArray[arrayIdx].data(),
			compressedSize, Z_BEST_COMPRESSION
		);

		m_compressedSizePerArray[arrayIdx] = compressedSize;
		compressedData.resize(compressedSize);

		result.push_back(move(compressedData));
	}

	return result;
}

vector<vector<uint8_t>> TextureAsset::DecompressDataArray()
{
	vector<vector<uint8_t>> result;
	for (unsigned int arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{
		uLongf originalSize = static_cast<uLong>(m_originalSizePerArray[arrayIdx]);
		uLongf compressSize = static_cast<uLong>(m_compressedSizePerArray[arrayIdx]);
		vector<uint8_t> originalData(originalSize);

		int compressResult = uncompress(
			originalData.data(),
			&originalSize,
			m_compressedBufferPerArray[arrayIdx].data(),
			compressSize
		);
		result.push_back(move(originalData));
	}
	return result;
}


void TextureAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeElement(m_width, fileIn);
	SerializeHelper::SerializeElement(m_height, fileIn);
	SerializeHelper::SerializeElement(m_arraySize, fileIn);

	SerializeHelper::SerializeSequenceContainer(m_originalSizePerArray, fileIn);
	SerializeHelper::SerializeSequenceContainer(m_compressedSizePerArray, fileIn);

	for (unsigned int arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{
		const vector<uint8_t> compressedBuffer = m_compressedBufferPerArray[arrayIdx];
		SerializeHelper::SerializeSequenceContainer(compressedBuffer, fileIn);
	}
}



void TextureAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	m_width = DeserializeHelper::DeserializeElement<decltype(m_width)>(fileIn);
	m_height = DeserializeHelper::DeserializeElement<decltype(m_height)>(fileIn);
	m_arraySize = DeserializeHelper::DeserializeElement<decltype(m_arraySize)>(fileIn);

	m_originalSizePerArray = DeserializeHelper::DeserializeSequenceContainer<vector<size_t>>(fileIn);
	m_compressedSizePerArray = DeserializeHelper::DeserializeSequenceContainer<vector<size_t>>(fileIn);

	for (unsigned int arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{
		m_compressedBufferPerArray.push_back(DeserializeHelper::DeserializeSequenceContainer<vector<uint8_t>>(fileIn));
	}
}
