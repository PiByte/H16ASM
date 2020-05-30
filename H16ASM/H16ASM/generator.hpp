#pragma once

/*
Generates code from parsed instructions
Returns binary in the form of a vector with bytes
*/

#include "parser.hpp"

#include <iostream>
#include <vector>

class Generator
{
public:
	
	std::vector<unsigned char> generate(std::vector<Instruction> input)
	{
		if (!input.size())
			std::cout << "(Generator) Empty input" << std::endl;

		std::vector<unsigned char> binary;
		unsigned char instByte = 0;

		// Goes through all instructions
		for (int i = 0; i < input.size(); i++)
		{
			// Different instructions have different lengths. The instruction type dictates what'll be written to the binary.
			switch (input[i].instructionType)
			{
				case InstructionTypes::SHORT: // Short inst. A single byte
					
					binary.push_back((input[i].opcode & 0b1111) << 2);

				break;
				case InstructionTypes::NORMAL:
					
					instByte = 0;
					instByte += (input[i].addressingMode & 0b11) << 6;
					instByte += (input[i].opcode & 0b1111) << 2;

					binary.push_back(instByte);

					switch (input[i].addressingMode) // Length of data also depends on addressing mode. When specifying an address, we need two bytes of data, otherwise we only need one.
					{
						default:
						case AddressingModes::REGISTER:
						case AddressingModes::IMMIDIETE:

							binary.push_back(input[i].data & 0xFF);

						break;

						case AddressingModes::DIRECT:
						case AddressingModes::INDIRECT:

							binary.push_back((input[i].data & 0xFF00) >> 8);
							binary.push_back(input[i].data & 0xFF);

						break;
					}

				break;
				case InstructionTypes::LONG: // The same as above more or less
					
					instByte = 0;
					instByte += (input[i].addressingMode & 0b11) << 6;
					instByte += (input[i].opcode & 0b1111) << 2;
					instByte += (input[i].switchData & 0b11);

					binary.push_back(instByte);

					switch (input[i].addressingMode)
					{
						default:
						case AddressingModes::REGISTER:
						case AddressingModes::IMMIDIETE:

							binary.push_back(input[i].data & 0xFF);

						break;

						case AddressingModes::DIRECT:
						case AddressingModes::INDIRECT:

							binary.push_back((input[i].data & 0xFF00) >> 8);
							binary.push_back(input[i].data & 0xFF);

						break;
					}

				break;
				case InstructionTypes::JUMP: // Always referes to address, so data is always 2 bytes

					instByte = 0;
					instByte += (input[i].addressingMode & 0b11) << 6;
					instByte += (input[i].opcode & 0b1111) << 2;
					instByte += (input[i].switchData & 0b11);

					binary.push_back(instByte);

					binary.push_back((input[i].data & 0xFF00) >> 8);
					binary.push_back(input[i].data & 0xFF);

				break;
			}
		}

		return binary;
	}
};