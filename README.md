# Pistone Compression Engine

## Overview
Pistone is a compression engine developed as part of a project to summarize knowledge acquired during the third semester of computer science studies. The project focuses on Huffman compression, offering functionalities for compression and decompression of files and folders. Compression options are selected via command-line arguments when invoking the program.

## Features
- Huffman compression algorithm implementation.
- Compression and decompression of both files and folders.
- Command-line interface for selecting compression options.

## Usage
To use Pistone, follow these steps:

1. Clone the repository to your local machine.
2. Navigate to the project directory.
3. Compile the source code using your preferred compiler, project is premake ready.
4. Run the executable with the appropriate command-line arguments to perform compression or decompression.

### Command-line Arguments
- `-i <file/folder>`: input path to file or folder.
- `-o <file/folder>`: output path of file or folder.
- `-m <huf>`: choose compression method, huf is default.
- `-f`: Enables compression/decompression for folders. If this option is specified, the input path should point to a folder.
- `-D`: Activates decoding mode
- `-E`: Activates encoding mode 

Example usage:
- .\Pistone.exe -i .\lorem.txt -o out_lorem.huf -E
- .\Pistone.exe -i .\lorem.huf -o out_lorem.txt -D
- .\Pistone.exe -i .\folder\ -o out_folder.hcd -E -f
- .\Pistone.exe -i .\in_folder.hcd -o .\out_folder\ -D -f
