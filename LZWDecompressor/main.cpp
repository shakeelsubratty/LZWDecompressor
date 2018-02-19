#include "stdafx.h"
#include "LZWDecompressor.h"
#include <iostream>

int main()
{
	LZWDecompressor lzwDecompressor;
	std::ifstream input;

	std::cout << "Please enter the file path of the file you want to decompress:" << std::endl;
	std::string filePath; 
	std::cin >> filePath;

	input.open(filePath, std::ios::binary | std::ios::in);
	if (input.is_open())
	{
		std::string output = lzwDecompressor.decompress(input);
		std::cout << output;
		input.close();
	}
	else
	{
		std::cout << "ERROR: File not found" << std::endl;
	}

	return 0;
};