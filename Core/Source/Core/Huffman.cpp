#include "Huffman.h"

namespace Huffman
{
	HuffmanNode::HuffmanNode(char character, int weight) : character(character), weight(weight), left(nullptr), right(nullptr) {}

	HuffmanNode::HuffmanNode(HuffmanNode* l, HuffmanNode* r) : character('\0'), weight(l->weight + r->weight), left(l), right(r) {}

	HuffmanNode::HuffmanNode(const HuffmanNode& other) : character(other.character), weight(other.weight), left(nullptr), right(nullptr)
	{
		if (other.left) {
			left = new HuffmanNode(*other.left);
		}
		if (other.right) {
			right = new HuffmanNode(*other.right);
		}
	}

	void HuffmanNode::getLeaves(HuffmanNode* root, std::unordered_map<char, String>& huffmanaCode, String pom)
	{
		if (root == nullptr)
			return;

		if (!root->left && !root->right) {
			huffmanaCode[root->character] = pom;
		}

		getLeaves(root->left, huffmanaCode, pom + "0");
		getLeaves(root->right, huffmanaCode, pom + "1");
	}

	HuffmanNode::~HuffmanNode()
	{
		delete left;
		delete right;
	};


	bool Comparator::operator() (HuffmanNode* left, HuffmanNode* right)
	{
		return left->weight > right->weight;
	}

	void CreateHeader(std::vector<std::bitset<8>>& encodedBits, const std::unordered_map<char, String>& huffmanCodes, int& bitIndex, std::size_t& byteIndex)
	{
		int numberOfCoddes  = huffmanCodes.size();
		if (numberOfCoddes > 65536) throw Core::CompressionException("Too many symbols to encode");

		encodedBits.push_back(std::bitset<8>(0));

		encodedBits.push_back(std::bitset<8>(numberOfCoddes & 0xFF));
		encodedBits.push_back(std::bitset<8>((numberOfCoddes >> 8) & 0xFF));

		bitIndex = 0;
		byteIndex = 3;
		encodedBits.push_back(std::bitset<8>());
		for (auto& pair : huffmanCodes)
		{

			for (int i = 0; i < 8; i++)
			{
				if (bitIndex == 8)
				{
					bitIndex = 0;
					++byteIndex;
					encodedBits.push_back(std::bitset<8>());
				}

				bool bit = (pair.first >> i) & 1;
				encodedBits[byteIndex][bitIndex] = bit;

				++bitIndex;
			}

			int codeLength = pair.second.length();
			char byteLegth = static_cast<char>(codeLength);

			for (int i = 0; i < 8; i++)
			{
				if (bitIndex == 8)
				{
					bitIndex = 0;
					++byteIndex;
					encodedBits.push_back(std::bitset<8>());
				}

				bool bit = (byteLegth >> i) & 1;
				encodedBits[byteIndex][bitIndex] = bit;

				++bitIndex;
			}

			for (int i = 0; i < codeLength; i++)
			{
				if (bitIndex == 8)
				{
					bitIndex = 0;
					++byteIndex;
					encodedBits.push_back(std::bitset<8>());
				}

				bool bit = pair.second[i] == '1';
				encodedBits[byteIndex][bitIndex] = bit;

				++bitIndex;
			}
		}
	}

	int ReadHeader(const std::vector<std::bitset<8>>& encodedBits, std::unordered_map<String, char>& huffmanCodes, int& bitIndex, std::size_t& byteIndex)
	{
		if (encodedBits.size() < 3)
		{
			throw Core::CompressionException("Invalid data format");
		}

		int bitsToTrim = static_cast<int>(encodedBits[0].to_ulong());

		int numberOfCoddes = (encodedBits[1].to_ulong()) | (encodedBits[2].to_ulong() << 8);

		byteIndex = 3;
		bitIndex = 0;

		for (int i = 0; i < numberOfCoddes; ++i)
		{
			char character = 0;
			for (int j = 0; j < 8; ++j)
			{
				if (bitIndex == 8)
				{
					bitIndex = 0;
					++byteIndex;
				}
				character |= (encodedBits[byteIndex][bitIndex] << j);
				++bitIndex;
			}

			int codeLength = 0;
			for (int j = 0; j < 8; ++j)
			{
				if (bitIndex == 8)
				{
					bitIndex = 0;
					++byteIndex;
				}
				codeLength |= (encodedBits[byteIndex][bitIndex] << j);
				++bitIndex;
			}

			String code;
			for (int j = 0; j < codeLength; ++j)
			{
				if (bitIndex == 8)
				{
					bitIndex = 0;
					++byteIndex;
				}
				code += (encodedBits[byteIndex][bitIndex] ? '1' : '0');
				++bitIndex;
			}

			huffmanCodes[code] = character;
		}

		return bitsToTrim;
	}

	void HuffmanCompression::encode(const std::shared_ptr<char>& data, std::size_t dataSize, std::vector<std::bitset<8>>& encodedData) const
	{
		std::unordered_map<char, int> wieghts;
		std::unordered_map<char, String> huffmanCodes;
		std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Comparator> pq;

		for (std::size_t i = 0; i < dataSize; i++)
		{
			char temp = data.get()[i];

			if (wieghts.contains(temp))
			{
				wieghts[temp] += 1;
			}
			else
			{
				wieghts[temp] = 1;
			}
		}

		for (auto& pair : wieghts)
		{
			HuffmanNode* n = new HuffmanNode(pair.first, pair.second);
			pq.push(n);
		}

		while (pq.size() > 1)
		{
			HuffmanNode* n1 = pq.top();
			pq.pop();


			HuffmanNode* n2 = pq.top();
			pq.pop();
			pq.push(new HuffmanNode(n1, n2));
		}

		HuffmanNode::getLeaves(pq.top(), huffmanCodes, "");

		int bitIndex = 0;
		std::size_t byteIndex;
		CreateHeader(encodedData, huffmanCodes, bitIndex, byteIndex);

		for (std::size_t i = 0; i < dataSize; i++)
		{
			String code = huffmanCodes[data.get()[i]];

			for (int j = 0; j < code.length(); j++)
			{
				if (bitIndex == 8)
				{
					bitIndex = 0;
					++byteIndex;
					encodedData.push_back(std::bitset<8>());
				}

				bool bit = code[j] == '1';
				encodedData[byteIndex][bitIndex] = bit;

				++bitIndex;
			}
		}

		int complement = bitIndex;
		encodedData[0] = std::bitset<8>(complement);
	}

	void HuffmanCompression::decode(const std::vector<std::bitset<8>>& dataToDecode, std::vector<char>& data) const
	{
		std::unordered_map<String, char> huffmanCodes;
		int bitIndex;
		std::size_t byteIndex;
		int bitsToTrim = ReadHeader(dataToDecode, huffmanCodes, bitIndex, byteIndex);



		String code = "";
		if (bitIndex == 8)
		{
			bitIndex = 0;
			++byteIndex;
		}

		std::size_t decodDataSize = dataToDecode.size();
		while (byteIndex < decodDataSize)
		{
			code += dataToDecode[byteIndex][bitIndex] ? "1" : "0";

			if (huffmanCodes.contains(code))
			{
				data.push_back(huffmanCodes[code]);
				code = "";
			}

			++bitIndex;
			if (bitIndex == 8)
			{
				bitIndex = 0;
				++byteIndex;
			}

			if (byteIndex == decodDataSize - 1 and bitIndex >= bitsToTrim) break;
		}
	}
}