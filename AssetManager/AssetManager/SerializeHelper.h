#pragma once
#include <iostream>
#include <string>
#include <vector>

class ISerializable
{
public:
	virtual void Serialize(FILE* fileIn) = 0;
	virtual void Deserialize(FILE* fileIn) = 0;
};

template <class T>
concept IsCountable = requires (T container)
{
	container.size();
};

class SerializeHelper
{
public:
	template<typename T>
	static void SerializeElement(const T& elementIn, FILE* fileIn);
	template<typename T>
	static void DeserializeElement(T& elementOut, FILE* fileIn);

public:
	static void SerializeString(const std::string& textIn, FILE* fileIn);
	static std::string DeserializeString(FILE* fileIn);

public:
	template<typename T>
	static void SerializeVectorHelper(const std::vector<T>& container, FILE* fileIn);
	template<typename T>
	static std::vector<T> DeserializeVectorHelper(FILE* fileIn);

public:
	static void SerializeContainerSizeHelper(const IsCountable auto& container, FILE* fileIn);
};

template<typename T>
inline void SerializeHelper::SerializeElement(const T& elementIn, FILE* fileIn)
{
	fwrite(&elementIn, sizeof(T), 1, fileIn);
}

template<typename T>
inline void SerializeHelper::DeserializeElement(T& elementOut, FILE* fileIn)
{
	fread(&elementOut, sizeof(T), 1, fileIn);
}

template<typename T>
inline void SerializeHelper::SerializeVectorHelper(const std::vector<T>& container, FILE* fileIn)
{
	const size_t containerSize = container.size();
	fwrite(&containerSize, sizeof(size_t), 1, fileIn);
	fwrite(container.data(), sizeof(T), containerSize, fileIn);
}

template<typename T>
inline std::vector<T> SerializeHelper::DeserializeVectorHelper(FILE* fileIn)
{
	std::vector<T> result;

	size_t containerSize;
	fread(&containerSize, sizeof(size_t), 1, fileIn);

	result.resize(containerSize);
	fread(result.data(), sizeof(T), containerSize, fileIn);

	return result;
}

inline void SerializeHelper::SerializeContainerSizeHelper(const IsCountable auto& container, FILE* fileIn)
{
	size_t containerCount = container.size();
	fwrite(&containerCount, sizeof(size_t), 1, fileIn);
}
