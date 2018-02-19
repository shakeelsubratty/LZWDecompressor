
#ifndef LZWDecompressor_H
#define LZWDecompressor_H

#include <vector>
#include <bitset>
#include <fstream>
#include <sstream>
#include <map>


/*
	Used as follows:
	...
	...
	LZWDecompressor lzw = LZWDecompressor();
	std::ifstream input;
	input.open(...);

	if(input.is_open())
	{
	std::string output = lzw.decompress(input);
	...
	...
	}
	...
	...
*/


const int codeSize = 12;
const int byteSize = 8;
const int initDictSize = 256;
const int dictMaxSize = 4096;

class LZWDecompressor
{
public:

	LZWDecompressor();
	std::string decompress(std::ifstream& input);

private:
	std::stringstream decompressedBuffer;
	std::map <int, std::string> dictionary;
	int dictHead;

	void insertToDictionary(std::string& word, char& ch);
	std::vector<std::bitset<codeSize>> readCodes(std::ifstream& input);
	void LZWDecompress(std::vector<std::bitset<codeSize>>& codes);
	std::string str();

};

#endif // !LZWDecompressor_H
