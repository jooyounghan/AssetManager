#include "pch.h"
#include "ATextureAsset.h"

using namespace std;

ATextureAsset::ATextureAsset(
	const string& assetNameIn,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	const unsigned int& arraysizeIn
) 
	: AAsset(assetNameIn), m_width(widthIn), m_height(heightIn), m_arraySize(arraysizeIn)
{

}

ATextureAsset::~ATextureAsset() 
{

}

void ATextureAsset::Serialize(FILE* fileIn) const
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



void ATextureAsset::Deserialize(FILE* fileIn)
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
