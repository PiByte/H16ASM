#pragma once

/*
Tokenizes the input stream
Returns tokens in vector
*/

#include <iostream>
#include <vector>

enum TokenTypes
{
	STRING,
	SYMBOL,
	NUMBER
};

struct Token
{
	std::string value;
	int type = 0;
};

class Tokenizer
{
public:

	std::vector<Token> tokenize(std::string code)
	{
		if (!code.length())
			std::cout << "(Tokenizer) Empty input" << std::endl;

		std::vector<Token> tokens;
		Token currentToken;
		int charCode;

		// Goes through the entire source file, looking for letters, numbers and symbols, and groups them together and categorizes them
		for (int i = 0; i < code.length(); i++)
		{
			charCode = (int)code.c_str()[i];

			// Checks for letters
			if (charCode >= (int)'A' && charCode <= (int)'Z' || charCode >= (int)'a' && charCode <= (int)'z')
			{
				currentToken.type = TokenTypes::STRING;
				currentToken.value += (char)charCode;

				continue;
			}

			// Checks for numbers
			if (charCode >= (int)'0' && charCode <= (int)'9')
			{
				currentToken.type = TokenTypes::NUMBER;
				currentToken.value += (char)charCode;

				continue;
			}

			// Checks for symbols
			if (charCode == (int)'$' || charCode == (int)'%' || charCode == (int)'&' || charCode == (int)'#' || charCode == (int)':')
			{
				if (currentToken.value.length())
					tokens.push_back(currentToken);

				currentToken.type = TokenTypes::SYMBOL;
				currentToken.value = (char)charCode;
				tokens.push_back(currentToken);

				currentToken.value = "";

				continue;
			}

			// Checks for comments
			if (charCode == (int)';')
			{
				if (currentToken.value.length())
					tokens.push_back(currentToken);

				currentToken.value = "";

				while (charCode != (int)'\n' && charCode != 0)
				{
					charCode = (int)code.c_str()[++i];
				}

				continue;
			}

			// Checks for whitespace (and commas). These characters will be ignored
			if (charCode == (int)' ' || charCode == (int)'\n' || charCode == (int)'\t' || charCode == (int)',')
			{
				if (currentToken.value.length())
					tokens.push_back(currentToken);

				currentToken.value = "";

				continue;
			}

			std::cout << "(Tokenizer) Unknown token: " << (char)charCode << std::endl;
			return tokens;
		}

		// Add the last token
		if (currentToken.value.length())
			tokens.push_back(currentToken);

		return tokens;
	}
};