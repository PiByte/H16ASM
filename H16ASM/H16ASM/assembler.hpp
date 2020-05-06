#pragma once

#include <iostream>
#include <vector>
#include <fstream>

struct Part
{
	std::string type;
	std::string value;
};

class Assembler
{
public:
	Assembler();
	~Assembler();

	void tokenize(std::string code)
	{
		std::vector<std::string> tokens;

		for (int i = 0; i < code.length(); i++)
		{
			std::string token;

			int charCode = (int)code.c_str()[i];

			if (charCode >= 65 && charCode <= 90 || charCode >= 97 && charCode <= 122 || charCode >= 48 && charCode <= 57) // Letters ad numbers
			{
				token += code.c_str()[i];
			}

			if (charCode >= 36 && charCode <= 38) // Special characters
			{
				tokens.push_back(token);

				token = code.c_str()[i];
				tokens.push_back(token);

				token = "";
			}

			if (charCode == 32 || charCode == 13 || charCode == 10) // Space or line break
			{
				tokens.push_back(token);
				token = "";
			}
			
		}
	}

	void parse(std::vector<std::string>*) {}

	void build(std::vector<Part>*) {}


private:


};