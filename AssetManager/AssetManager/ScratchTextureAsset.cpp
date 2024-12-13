#include "pch.h"
#include "ScratchTextureAsset.h"
#include "AssetException.h"

using namespace std;
using namespace DirectX;

ScratchTextureAsset::ScratchTextureAsset(
	const string& assetName, 
	const ScratchImage& scratch,
	const TexMetadata& metaData
)
	: ATextureAsset(
		assetName, 
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
			m_rowPitchPerArray.push_back(static_cast<UINT>(img->rowPitch));
		}
		else
		{
			originalBufferPerArray.push_back(vector<uint8_t>());
			m_originalSizePerArray.push_back(NULL);
			m_rowPitchPerArray.push_back(NULL);
			AssetException assetException(*this, "ScratchImage::GetImage's Result (DirectX::Image) is nullptr");
			throw assetException;
		}

	}
	m_compressedBufferPerArray = CompressDataArray(originalBufferPerArray);
}

ScratchTextureAsset::~ScratchTextureAsset()
{
}

std::vector<uint32_t> ScratchTextureAsset::GetRowPitchArray()
{
	return m_rowPitchPerArray;
}

void ScratchTextureAsset::Serialize(FILE* fileIn) const
{
	ATextureAsset::Serialize(fileIn);
	SerializeHelper::SerializeSequenceContainer(m_rowPitchPerArray, fileIn);
}

void ScratchTextureAsset::Deserialize(FILE* fileIn)
{
	ATextureAsset::Deserialize(fileIn);
	m_rowPitchPerArray = DeserializeHelper::DeserializeSequenceContainer<vector<UINT>>(fileIn);
}
