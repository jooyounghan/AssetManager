#include "pch.h"
#include "ATextureAsset.h"

using namespace std;

ATextureAsset::ATextureAsset(
	const string& assetPathIn, 
	const string& assetNameIn,
	const unsigned int& widthIn,
	const unsigned int& heightIn,
	const unsigned int& arraysizeIn
) 
	: AAsset(assetPathIn, assetNameIn), m_width(widthIn), m_height(heightIn), m_arraySize(arraysizeIn)
{

}

ATextureAsset::~ATextureAsset() 
{

}

void ATextureAsset::Serialize(FILE* fileIn)
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeElement(m_width, fileIn);
	SerializeHelper::SerializeElement(m_height, fileIn);
	SerializeHelper::SerializeElement(m_arraySize, fileIn);

	SerializeHelper::SerializeVectorHelper(m_originalSizePerArray, fileIn);
	SerializeHelper::SerializeVectorHelper(m_compressedSizePerArray, fileIn);

	for (unsigned int arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{
		const vector<uint8_t> compressedBuffer = m_compressedBufferPerArray[arrayIdx];
		SerializeHelper::SerializeVectorHelper(compressedBuffer, fileIn);
	}
}



void ATextureAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);

	SerializeHelper::DeserializeElement(m_width, fileIn);
	SerializeHelper::DeserializeElement(m_height, fileIn);
	SerializeHelper::DeserializeElement(m_arraySize, fileIn);

	m_originalSizePerArray = SerializeHelper::DeserializeVectorHelper<size_t>(fileIn);
	m_compressedSizePerArray = SerializeHelper::DeserializeVectorHelper<size_t>(fileIn);

	for (unsigned int arrayIdx = 0; arrayIdx < m_arraySize; ++arrayIdx)
	{
		m_compressedBufferPerArray.push_back(SerializeHelper::DeserializeVectorHelper<uint8_t>(fileIn));
	}
}
