#pragma once

/*
Runs the asssembler. This is just here to keep the code slightly cleaner. lol
*/

#include "tokenizer.hpp"
#include "parser.hpp"
#include "generator.hpp"

#include <iostream>
#include <vector>

class Assembler
{
public:
	
	std::vector<unsigned char> assemble(std::string code)
	{
		Tokenizer t;
		Parser p;
		Generator g;

		return g.generate(p.parse(t.tokenize(code)));
	}
};