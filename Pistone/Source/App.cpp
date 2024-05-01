#include "App.h"
#include "Core/Core.h"
#include "Core/Huffman.h"
#include <iostream>


int main(int argc, char* argv[])
{
	String inFilePath = "";
	String outFilePath = "";
	bool encodingMode = true;
	bool isDirectory = false;
	std::unique_ptr<Core::CompressionMethod> compressionMethod;

	if (argc <= 1)
	{
		PRINT_HELP;
		return 0;
	}

	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{

			switch (argv[i][1])
			{
				case 'i':
					if (i < argc - 1)
					{
						inFilePath = argv[++i];
					}
					break;

				case 'o':
					if (i < argc - 1)
					{
						outFilePath = argv[++i];
					}
					break;

				case 'E':
						encodingMode = true;
						break;

				case 'f':
						isDirectory = true;
						break;

				case 'D':
						encodingMode = false;
						break;

				case 'm':
					if (strcmp(argv[i], "-m") == 0)
					{
						if (i < argc - 1)
						{
							if (strcmp(argv[i], "huf") == 0)
							{
								Huffman::HuffmanCompression huf = Huffman::HuffmanCompression();
								compressionMethod = std::make_unique<Huffman::HuffmanCompression>(huf);
							}
						}
					}
				case 'h':
					if (strcmp(argv[i], "-man") == 0 ||
						strcmp(argv[i], "-h") == 0   ||
						strcmp(argv[i], "-help") == 0 )
					{
						PRINT_HELP;
						return 0;
					}
					break;

				default:
					break;
			}

		}

	}

	if (inFilePath == "")
	{
		std::cout << "No file path provided." << std::endl;
		return 1;
	}
	else
	{
		if (encodingMode)
		{		
			try 
			{
				std::shared_ptr<char> data;
				std::size_t size;
				if (isDirectory)
				{
					Core::ReadFolder(inFilePath, data, size);
				}
				else
				{
					Core::ReadFile(inFilePath, data, size);
				}
				
				
				//Guessing the sufficient length of the vector
				std::vector<std::bitset<8>> encodedData;
				encodedData.reserve(round(size * 0.9));

				compressionMethod->encode(data, size, encodedData);
				Core::WriteFile(outFilePath, encodedData);
			}
			catch (const Core::CompressionException& error)
			{
				std::cout << error.what() << std::endl;
				return 1;
			}
		}
		else
		{
			try
			{
				std::vector<std::bitset<8>> dataToDecodede;
				Core::ReadFile(inFilePath, dataToDecodede);
 
				std::vector<char> data;
				data.reserve(dataToDecodede.size() * 4);

				compressionMethod->decode(dataToDecodede, data);

				if (isDirectory)
				{
					Core::WriteFolder(outFilePath, data);
				}
				else
				{
					Core::WriteFile(outFilePath, data);
				}
			}
			catch (const Core::CompressionException& error)
			{
				std::cout << error.what() << std::endl;
				return 1;
			}
		}		
	}
	return 0;
}