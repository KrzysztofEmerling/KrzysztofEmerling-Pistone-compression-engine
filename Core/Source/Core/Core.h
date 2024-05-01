#pragma once

#include <fstream>
#include <filesystem>
#include <exception>
#include <memory>
#include <bitset>
#include <vector>
#include <string> 

#define fs std::filesystem
#define String std::string

namespace Core
{
	/**
	* @brief An interface to compression methods.
	*/
	class CompressionMethod {
	public:
		virtual void encode(const std::shared_ptr<char>& data, std::size_t dataSize, std::vector<std::bitset<8>>& encodedData) const = 0;
		virtual void decode(const std::vector<std::bitset<8>>& dataToDecode, std::vector<char>& data) const = 0;
		virtual ~CompressionMethod() {} // Wirtualny destruktor
	};



	/**
	 * @brief A custom exception class for compression-related errors.
	 *
	 * This class inherits from std::exception and provides a mechanism to throw
	 * exceptions specifically related to compression operations.
	 */
	class CompressionException : public std::exception {
	private:
		/**< The error message associated with the exception. */
		String error;

	public:
		/**
		 * @brief Constructs a CompressionException with the given error message.
		 * @param msg The error message to associate with the exception.
		 */
		CompressionException(const String& msg) : error(msg) {}

		/**
		 * @brief Returns a C-style string describing the exception.
		 *
		 * This function overrides the what() method of std::exception to provide
		 * a custom error message associated with the exception.
		 *
		 * @return A pointer to a null-terminated string describing the exception.
		 */
		const char* what() const noexcept override;
	};

	/**
	 * @brief Reads the contents of a file into a shared pointer to char array.
	 *
	 * @param filepath The path to the file to be read.
	 * @param data The shared pointer to char array to store the file data.
	 * @param size Reference to store the size of the file.
	 *
	 * @throw CompressionException if there is an error loading the file.
	 */
	void ReadFile(const String& filepath, std::shared_ptr<char>& data, std::size_t& size);

	/**
	* @brief Reads the contents of a file into a vector of bitsets.
	*
	* Each bitset contains 8 bits representing a byte from the file.
	*
	* @param filepath The path to the file to be read.
	* @param data The vector of bitsets to store the file data.
	*
	* @throw std::runtime_error if there is an error loading the file.
	*/
	void ReadFile(const String& filepath, std::vector<std::bitset<8>>& data);

	std::size_t readSubfolders(const fs::path& folderPath, String& folderStructure);

	/**
	* @brief Reads the contents of a folder into a shared pointer to char array.
	*
	* This function constructs a string pattern representing the folder contents
	* and stores it in the provided data pointer. Each character in the pattern
	* represents a specific type of file or subfolder.
	*
	* @param folderPath The path to the folder to be read.
	* @param data The shared pointer to char array to store the folder contents pattern.
	* @param size Reference to store the size of the folder contents pattern.
	*/
	void ReadFolder(const String& folderPath, std::shared_ptr<char>& data, std::size_t& size);

	/**
	 * @brief Writes data from a shared pointer to a file.
	 *
	 * @param filepath The path to the file to write.
	 * @param data The shared pointer to the data to write.
	 * @param size The size of the data.
	 *
	 * @throw CompressionException if there is an error opening the file.
	 */
	void WriteFile(const String& filepath, std::shared_ptr<char>& data, std::size_t size);

	/**
	* @brief Writes data from a vector of characters to a file.
	*
	* @param filepath The path to the file to write.
	* @param data The vector of characters to write.
	*
	* @throw CompressionException if there is an error opening the file.
	*/
	void WriteFile(const String& filepath, const std::vector<char>& data);

	/**
	* @brief Writes data from a vector of bitsets to a file.
	*
	* Each bitset represents a byte in the file.
	*
	* @param filepath The path to the file to write.
	* @param data The vector of bitsets to write.
	*
	* @throw std::runtime_error if there is an error opening the file.
	*/
	void WriteFile(const String& filepath, const std::vector<std::bitset<8>>& data);

	/**
	* @brief Writes the contents of a folder recursively based on the provided contents.
	*
	* This function parses the provided contents vector to reconstruct the folder structure
	* and writes the contents of each file or subfolder to the specified path recursively.
	*
	* @param path The path to the folder where the contents will be written.
	* @param contents The vector containing the compressed folder contents.
	*
	* @throw CompressionException if there is an error in the folder compression format or writing files.
	*/
	void WriteFolder(const String& path, std::vector<char>& contents);
	
}