#pragma once

#include"Core.h"
#include <unordered_map> 
#include <queue>



namespace Huffman
{
	/**
	* @brief Represents a node in a Huffman tree.
	*/
	class HuffmanNode {
	public:

		char character;

		int weight;
	private:
		HuffmanNode* left;

		HuffmanNode* right;
	public:
		/**
		* @brief Constructs a HuffmanNode with a character and weight.
		*
		* @param character The character stored in the node.
		* @param weight The weight of the node.
		*/
		HuffmanNode(char character, int weight);

		/**
		* @brief Constructs a HuffmanNode with left and right child nodes.
		*
		* @param l Pointer to the left child node.
		* @param r Pointer to the right child node.
		*/
		HuffmanNode(HuffmanNode* l, HuffmanNode* r);

		/**
		* @brief Copy constructor.
		*
		* @param other The HuffmanNode to copy.
		*/
		HuffmanNode(const HuffmanNode& other);

		/**
		* @brief Static method to generate Huffman codes for leaf nodes.
		*
		* @param root Pointer to the root node of the Huffman tree.
		* @param huffmanCode Map to store Huffman codes for characters.
		* @param pom Temporary string for generating Huffman codes.
		*/
		static void getLeaves(HuffmanNode* root, std::unordered_map<char, String>& huffmanaCode, String pom);

		/**
		* @brief Destructor.
		*/
		~HuffmanNode();
	};

	/**
	* @brief Functor for comparing HuffmanNode pointers.
	*/
	struct Comparator
	{
		/**
		* @brief Compares two HuffmanNode pointers, left->weight > right->weight.
		*
		* @param left Pointer to the left HuffmanNode.
		* @param right Pointer to the right HuffmanNode.
		* @return True if the weight of the left node is greater than the weight of the right node, false otherwise.
		*/
		bool operator()(HuffmanNode* left, HuffmanNode* right);
	};

	/**
	* @brief Creates the header for Huffman encoding.
	*
	* The header format:
	* - 8 bits: bits to trim from the last byte
	* - 16 bits: number of Huffman codes
	* - For each code:
	*   - 8 bits: character
	*   - 8 bits: number of bits in the code
	*   - Actual bits representing the code
	*
	* @param encodedBits Vector to store the encoded header bits.
	* @param huffmanCodes Map containing Huffman codes for characters.
	* @param bitIndex Reference to the current bit index within the byte.
	* @param byteIndex Reference to the current byte index within the vector.
	*
	* @throw Core::CompressionException if there are too many symbols to encode.
	*/
	void CreateHeader(std::vector<std::bitset<8>>& encodedBits, const std::unordered_map<char, String>& huffmanCodes, int& bitIndex, std::size_t& byteIndex);

	/**
	* @brief Reads the header for Huffman encoding.
	*
	* The header format:
	* - 8 bits: bits to trim from the last byte
	* - 16 bits: number of Huffman codes
	* - For each code:
	*   - 8 bits: character
	*   - 8 bits: number of bits in the code
	*   - Actual bits representing the code
	*
	* @param encodedBits Vector containing the encoded header bits.
	* @param huffmanCodes Map to store Huffman codes for characters.
	* @param bitIndex Reference to the current bit index within the byte.
	* @param byteIndex Reference to the current byte index within the vector.
	* @return The number of bits to trim from the last byte.
	*
	* @throw Core::CompressionException if the data format is invalid.
	*/
	int ReadHeader(std::vector<std::bitset<8>>& encodedBits, const std::unordered_map<String, char>& huffmanCodes, int& bitIndex, std::size_t& byteIndex);

	/**
	* @brief A class containing Huffman compression method.
	*/
	class HuffmanCompression : public Core::CompressionMethod
	{
	public:

		HuffmanCompression() = default;

		/**
		* @brief encodes the given data using Huffman coding.
		*
		* @param data The shared pointer to the data to be encoded.
		* @param dataSize The size of the data.
		* @param encodedData Vector to store the encoded data.
		*
		* This function performs Huffman encoding on the input data and stores the encoded bits in the provided vector.
		*
		* @throw Core::CompressionException if there is an error during encoding.
		*/
		void encode(const std::shared_ptr<char>& data, std::size_t dataSize, std::vector<std::bitset<8>>& encodedData) const override;

		/**
		* @brief decodes the given encoded data using Huffman coding.
		*
		* @param dataToDecode The vector containing the encoded data to be decoded.
		* @param data Vector to store the decoded data.
		*
		* This function decodes the input data using the provided Huffman codes and stores the decoded characters in the output vector.
		*
		* @throw Core::CompressionException if there is an error during decoding.
		*/
		void decode(const std::vector<std::bitset<8>>& dataToDecode, std::vector<char>& data) const override;
	};
}