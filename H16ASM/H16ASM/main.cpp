/*
Main file, loads in source code, builds it and writes the final binray to a file
*/

#include <iostream>
#include <fstream>
#include <streambuf>

#include "assembler.hpp"

int main(int argc, char** argv)
{
	std::string logo = "______  __                           _____________\n___  / / /____  _______________________<  /_  ___/\n__  /_/ /__  / / /__  __ \\  _ \\_  ___/_  /_  __ \\ \n_  __  / _  /_/ /__  /_/ /  __/  /   _  / / /_/ / \n/_/ /_/  _\\__, / _  .___/\\___//_/    /_/  \\____/  \n         /____/  /_/                              \n                                                  \n______ ______________ ___                         \n_  __ `/_  ___/_  __ `__ \\                        \n/ /_/ /_(__  )_  / / / / /                        \n\\__,_/ /____/ /_/ /_/ /_/                         ";

	std::cout << logo << "\nVersion 1.0\n\n";

	if (argc < 2)
	{
		std::cout << "(Main) Please specify file to compile" << std::endl;
		return 0;
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
	Assembler a;
	std::vector<unsigned char> binary = a.assemble(code);

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