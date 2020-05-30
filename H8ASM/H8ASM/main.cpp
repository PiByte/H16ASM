/*
Main file, loads in source code, builds it and writes the final binray to a file
*/

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string> // for std::stoi
#include <stdexcept> // for catching std::stoi errors

#include "tokenizer.hpp"
#include "parser.hpp"
#include "generator.hpp"

int main(int argc, char** argv)
{
	std::string logo = "   __ __                  ___ \n  / // /_ _____  ___ ____( _ )\n / _  / // / _ \\/ -_) __/ _  |\n/_//_/\\_, / .__/\\__/_/  \\___/ \n ___ /___/_/ _                \n/ _ `(_-</  ' \\               \n\\_,_/___/_/_/_/               ";
	std::cout << logo << "\nVersion 1.2\n\n";

	if (argc < 2)
	{
		std::cout << "(Main) Please specify file to compile" << std::endl;
		return 0;
	}

	// Starting address
	int stAddr;
	if (argc < 3)
	{
		std::cout << "(Main) Starting address missing, defaulting to 0x0400" << std::endl;
		stAddr = 0x0400;
	}
	else
	{
		try {
			stAddr = std::stoi(std::string(argv[2]));
		}
		catch(const std::invalid_argument& ia)
		{
			std::cout << "(Main) " << ia.what() << std::endl;
			return 0;
		}
	}
	
	// Loads the source code
	std::fstream inputFile(argv[1], std::fstream::in);
	std::string code;

	if (inputFile.is_open())
	{
		code = std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
	}
	else
	{
		std::cout << "(Main) File not found!" << std::endl;
		return 0;
	}
	inputFile.close();

	// Runs the assembler
	Tokenizer t;
	Parser p;
	Generator g;

	std::vector<unsigned char> binary = g.generate(p.parse(t.tokenize(code), stAddr));

	// Outputs file
	std::fstream outputFile(std::string(argv[1]) + ".bin", std::fstream::out | std::fstream::binary);
	if (outputFile.is_open())
	{
		for (int i = 0; i < binary.size(); i++)
			outputFile << (char)binary[i];
	}
	else
	{
		std::cout << "(Main) Could not write file to disk!" << std::endl;
		return 0;
	}
	outputFile.close();

	std::cout << "Binary written to " << std::string(argv[1]) + ".bin" << std::endl;

	// bye bye
	return 0;
}