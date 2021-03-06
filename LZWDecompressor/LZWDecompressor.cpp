// LZWDecompressor.cpp : 

#include "stdafx.h"
#include "LZWDecompressor.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>
#include <vector>

using namespace std;

LZWDecompressor::LZWDecompressor()
{	
	//Initialise and populate dictionary with 256 ASCII characters
	dictionary = map<int,string>();
	for (unsigned long i = 0; i < initDictSize; i++)
	{
		dictionary.insert(std::pair<unsigned long, string>(i, string(1, static_cast<char>(i))));
	}
	dictHead = initDictSize;
	decompressedBuffer = stringstream();
}


string LZWDecompressor::decompress(ifstream& input)
{
	vector<bitset<codeSize>> codes = readCodes(input);
	LZWDecompress(codes);
	return str();
}

string LZWDecompressor::str()
{
	return decompressedBuffer.str();
}


vector<bitset<codeSize>> LZWDecompressor::readCodes(ifstream& input)
{
	//Read a byte into mem[0]
	char mem[1];
	input.read(mem, sizeof(mem));

	bitset<codeSize> buffer;
	bitset<byteSize> currentByte(mem[0]);
	vector<bitset<codeSize>> codes;

	int memPos = byteSize;
	int bufferPos = codeSize;

	while (!input.eof())
	{
		memPos--;
		bool bit = currentByte.test(memPos);
		//When whole byte is read, read the next byte
		if (memPos <= 0)
		{
			input.read(mem, sizeof(mem));
			currentByte = bitset<byteSize>(mem[0]);
			memPos = byteSize;
		}

		bufferPos--;
		buffer.set(bufferPos, bit);
		//When 12 bits of buffer is full, store code in vector and reset
		if (bufferPos <= 0)
		{
			codes.push_back(buffer);
			buffer.reset();
			bufferPos = codeSize;
		}
	}

	input.close();
	return codes;
}

void LZWDecompressor::LZWDecompress(vector<bitset<codeSize>>& codes)
{
	bitset<codeSize> oldCode;
	bitset<codeSize> currCode;
	char ch;
	string entry;

	oldCode = codes[0];
	decompressedBuffer << dictionary.at(oldCode.to_ulong());

	for (vector<bitset<codeSize>>::iterator i = ++codes.begin(); i != codes.end(); i++)
	{
		currCode = *i;
		unsigned long v = currCode.to_ulong();

		string w = dictionary.at(oldCode.to_ulong());

		/*
		The case where the code == head of dictionary (therefore code is not in dictionary) can only occur when the substring begins 
		and ends with the same character.
		*/
		if (v == dictHead)
		{
			entry = w + w[0];
		}
		else
		{
			entry = dictionary.at(v);
		}
		decompressedBuffer << entry;

		ch = entry[0];
		insertToDictionary(w, ch);
		oldCode = currCode;
	}

}

void LZWDecompressor::insertToDictionary(string& word, char& ch)
{
	//Reset dictionary if full
	if (dictHead == dictMaxSize)
	{
		dictionary.clear();
		for (unsigned long i = 0; i < initDictSize; i++)
		{
			dictionary.insert(std::pair<unsigned long, string>(i, string(1, static_cast<char>(i))));
		}
		dictHead = initDictSize;
	}
	dictionary.insert(pair<int, string>(dictHead, word.append(1, ch)));
	dictHead++;
}



