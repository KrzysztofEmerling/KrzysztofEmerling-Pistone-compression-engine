#include "Core.h"

namespace Core {

	const char* Core::CompressionException::what() const noexcept
	{
		return error.c_str();
	}


	void ReadFile(const String& filepath, std::shared_ptr<char>& data, std::size_t& size)
	{
		std::ifstream plikIn;
		plikIn.open(filepath, std::ios::binary);
		if (plikIn.good())
		{
			plikIn.seekg(0, std::ios::end);
			std::streampos fileSize = plikIn.tellg();
			size = static_cast<std::size_t>(fileSize);
			plikIn.seekg(0, std::ios::beg);

			data = std::shared_ptr<char>(new char[size], std::default_delete<char[]>());

			plikIn.read(data.get(), fileSize);

			plikIn.close();
		}
		else throw CompressionException("Error loading: " + filepath);
	}



	void ReadFile(const String& filepath, std::vector<std::bitset<8>>& data)
	{
		std::ifstream file(filepath, std::ios::binary);
		if (file)
		{
			file.seekg(0, std::ios::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios::beg);

			data.resize(fileSize);

			char byte;
			for (std::size_t i = 0; i < fileSize; ++i)
			{
				file.read(&byte, 1);
				data[i] = std::bitset<8>(byte);
			}

			file.close();
		}
		else
		{
			throw std::runtime_error("Error loading: " + filepath);
		}
	}

	std::size_t readSubfolders(const fs::path& folderPath, String& folderStructure)
	{
		std::size_t startSize = folderStructure.length();

		for (const auto& entry : fs::directory_iterator(folderPath))
		{
			if (entry.is_directory())
			{
				folderStructure += entry.path().filename().string() + "\\*________";
				std::size_t index = folderStructure.length() - 8;
				std::size_t temp = readSubfolders(entry, folderStructure);
				for (int offset = 0; offset < 8; offset++) folderStructure[index + offset] = static_cast<char>((temp >> (offset * 8)) & 0xFF);

			}
			else if (entry.is_regular_file())
			{
				String filename = entry.path().filename().string();
				std::size_t size = 0;
				std::shared_ptr<char> data;
				ReadFile(entry.path().string(), data, size);

				std::size_t temp = size;
				String len = "________";
				for (int offset = 0; offset < 8; offset++) len[offset] = static_cast<char>((temp >> (offset * 8)) & 0xFF);
 
				folderStructure += filename + "*" + len + String(data.get(), size);
			}
		}
		return folderStructure.length() - startSize;
	}


	void ReadFolder(const String& folderPath, std::shared_ptr<char>& data, std::size_t& size)
	{
		String str = folderPath.substr(2);
		str.erase(str.size());
		str += "*________";
		std::size_t index = str.length() - 8;
		std::size_t temp = readSubfolders(folderPath, str);
		for (int offset = 0; offset < 8; offset++) str[index + offset] = static_cast<char>((temp >> (offset * 8)) & 0xFF);

		
		size = str.size();
		char* newData = new char[size];
		std::memcpy(newData, str.c_str(), size);

		data.reset(newData);
	}

	void WriteFile(const String& filepath, std::shared_ptr<char>& data, std::size_t size)
	{
		std::ofstream plikOut(filepath, std::ios::binary);

		if (plikOut.good())
		{
			plikOut.write(data.get(), size);
			plikOut.close();
		}
		else
		{
			throw CompressionException("Error opening file:: " + filepath);
		}
	}

	void WriteFile(const String& filepath, const std::vector<char>& data)
	{
		std::ofstream file(filepath, std::ios::out | std::ios::binary);

		if (file.is_open()) {
			file.write(data.data(), data.size());
			file.close();
		}
		else throw CompressionException("Error opening file:: " + filepath);
	}

	void WriteFile(const String& filepath, const std::vector<std::bitset<8>>& data)
	{
		std::ofstream outputFile(filepath, std::ios::binary);

		if (!outputFile.is_open()) {
			throw std::runtime_error("Error opening file: " + filepath);
		}

		for (const auto& bitset : data)
		{
			char byte = static_cast<char>(bitset.to_ulong());
			outputFile.write(&byte, 1);
		}

		outputFile.close();
	}

	void WriteFolder(const String& path, std::vector<char>& contents)
	{
		auto beginOfParcedChars = contents.begin();
		auto endOfParcedChars = contents.end();
		auto wsk = std::find(beginOfParcedChars, endOfParcedChars, '*');
		String filepath;

		while (wsk != endOfParcedChars)
		{
			filepath = String(beginOfParcedChars, wsk);
			if (wsk + 8 >= endOfParcedChars)throw CompressionException("Invalid folder compresion format");
			String temp = String(wsk + 1,wsk + 9);
			std::size_t parcedContedSize = 0;

			for (int offset = 0; offset < 8; offset++)
			{
				std::bitset<8> b(temp[offset]);
				parcedContedSize |= b.to_ullong() << (offset * 8);
			}
			if (wsk + parcedContedSize + 8 >= endOfParcedChars) throw CompressionException("Index out of range ...");

			std::vector<char> parcedContent(wsk + 9, wsk + 9 + parcedContedSize);
			beginOfParcedChars = wsk + 9 + parcedContedSize;
			if (*(wsk - 1) == '\\')
			{
				fs::create_directory(path + filepath);
				WriteFolder(path + filepath, parcedContent);
			}
			else
			{
				WriteFile(path + filepath, parcedContent);
			}

			wsk = std::find(beginOfParcedChars, endOfParcedChars, '*');
		}

	}	
	
}