#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

class ISerializable
{
public:
	virtual void Serialize(FILE* fileIn) const = 0;
	virtual void Deserialize(FILE* fileIn) = 0;
};

#pragma region Concept
template <typename T>
concept SerializablePrimitive = std::is_arithmetic_v<T> ||
(std::is_class_v<T> && std::is_trivial_v<T> && std::is_standard_layout_v<T>);

template <typename T>
concept Serializable = std::is_base_of_v<ISerializable, T>;

template <class T>
concept IsCountable = requires (T container)
{
	container.size();
};

template <typename T>
concept SequenceContainer = IsCountable<T> && requires(T container) 
{
	typename T::value_type;
	{ container.begin() } -> std::input_iterator;
} && SerializablePrimitive<typename T::value_type>;

template <typename T>
concept KeyValueContainer = IsCountable<T> && requires(T container) 
{
	typename T::key_type;
	typename T::mapped_type;
	typename T::value_type;
	{ container.begin() } -> std::input_iterator;
} && SerializablePrimitive<typename T::key_type> && SerializablePrimitive<typename T::mapped_type>;
#pragma endregion

class SerializeHelper
{
public:
	template <SerializablePrimitive T>
	static void SerializeElement(const T& elementIn, FILE* fileIn);

public:
	static void SerializeString(const std::string& textIn, FILE* fileIn);

	template <SequenceContainer Container>
	static void SerializeSequenceContainer(const Container& container, FILE* fileIn);

public:
	template <KeyValueContainer Container>
	static void SerializeKeyValueContainer(const Container& container, FILE* fileIn);

public:
	template<IsCountable Container>
	static void SerializeContainerSize(const Container& container, FILE* fileIn);
};

class DeserializeHelper
{
public:
	template <SerializablePrimitive T>
	static T DeserializeElement(FILE* fileIn);

public:
	static std::string DeserializeString(FILE* fileIn);

public:
	template <SequenceContainer Container>
	static Container DeserializeSequenceContainer(FILE* fileIn);

public:
	template <KeyValueContainer Container>
	static Container DeserializeKeyValueContainer(FILE* fileIn);

};

template <SerializablePrimitive T>
inline void SerializeHelper::SerializeElement(const T& elementIn, FILE* fileIn)
{
	fwrite(&elementIn, sizeof(T), 1, fileIn);
}

template <SerializablePrimitive T>
inline T DeserializeHelper::DeserializeElement(FILE* fileIn)
{
	T value;
	fread(&value, sizeof(T), 1, fileIn);
	return value;
}

template<SequenceContainer Container>
inline void SerializeHelper::SerializeSequenceContainer(const Container& container, FILE* fileIn)
{
	using ValueType = typename Container::value_type;

	const size_t containerSize = container.size();
	SerializeHelper::SerializeElement(containerSize, fileIn);

	for (const auto& element : container)
	{
		SerializeHelper::SerializeElement(element, fileIn);
	}
}

template<SequenceContainer Container>
inline Container DeserializeHelper::DeserializeSequenceContainer(FILE* fileIn)
{
	using ValueType = typename Container::value_type;

	size_t containerSize = DeserializeElement<size_t>(fileIn);

	Container result;
	for (size_t containerIdx = 0; containerIdx < containerSize; ++containerIdx)
	{
		ValueType value = DeserializeElement<ValueType>(fileIn);
		result.push_back(value);
	}

	return result;
}


template<KeyValueContainer Container>
inline void SerializeHelper::SerializeKeyValueContainer(const Container& container, FILE* fileIn) 
{
	using KeyType = typename Container::key_type;
	using ValueType = typename Container::mapped_type;

	const size_t containerSize = container.size();
	SerializeElement(containerSize, fileIn);

	for (const auto& element : container) 
	{
		SerializeElement(element.first, fileIn);
		SerializeElement(element.second, fileIn);
	}
}

template<KeyValueContainer Container>
inline Container DeserializeHelper::DeserializeKeyValueContainer(FILE* fileIn) 
{
	using KeyType = typename Container::key_type;
	using ValueType = typename Container::mapped_type;

	size_t containerSize = DeserializeElement<size_t>(fileIn);

	Container result;
	for (size_t containerIdx = 0; containerIdx < containerSize; ++containerIdx) 
	{
		KeyType key = DeserializeElement<KeyType>(fileIn);
		ValueType value = DeserializeElement<ValueType>(fileIn);
		result.emplace(key, value);
	}

	return result;
}

template<IsCountable Container>
inline void SerializeHelper::SerializeContainerSize(const Container& container, FILE* fileIn)
{
	const size_t containerSize = container.size();
	SerializeElement(containerSize, fileIn);
}
