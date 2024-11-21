#include "pch.h"
#include "ScratchTextureAsset.h"
#include "AssetException.h"

using namespace std;
using namespace DirectX;

ScratchTextureAsset::ScratchTextureAsset(
	const string& assetPathIn,
	const string& assetNameIn
)
	: ATextureAsset(assetPathIn, assetNameIn, 0, 0, 0)
{

}

ScratchTextureAsset::ScratchTextureAsset(
	const string& assetPathIn, 
	const string& assetNameIn, 
	const ScratchImage& scratch,
	const TexMetadata& metaData
)
	: ATextureAsset(
		assetPathIn, assetNameIn, 
		static_cast<unsigned int>(metaData.width), 
		static_cast<unsigned int>(metaData.height),
		static_cast<unsigned int>(metaData.arraySize)
	)
{
	vector<vector<uint8_t>> originalBufferPerArray;

	for (size_t ArrayIdx = 0; ArrayIdx < m_arraySize; ++ArrayIdx)
	{
		const Image* img = scratch.GetImage(0, ArrayIdx, 0);

		if (img != nullptr)
		{
			const size_t originalBufferSize = img->rowPitch * img->height;
			vector<uint8_t> originalBuffer(originalBufferSize);
			memcpy(originalBuffer.data(), img->pixels, originalBufferSize);
			m_originalSizePerArray.push_back(originalBufferSize);
		}
		else
		{
			originalBufferPerArray.push_back(vector<uint8_t>());
			m_originalSizePerArray.push_back(NULL);

			AssetException assetException(*this, "ScratchImage::GetImage's Result (DirectX::Image) is nullptr");
			throw assetException;
		}

	}
	m_compressedBufferPerArray = CompressDataArray(originalBufferPerArray);
}

ScratchTextureAsset::~ScratchTextureAsset()
{
}

vector<vector<uint8_t>> ScratchTextureAsset::CompressDataArray(
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

vector<vector<uint8_t>> ScratchTextureAsset::DecompressDataArray()
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
