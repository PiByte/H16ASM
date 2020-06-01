#pragma once

/*
Parses tokens
Returns deconstructed instrucions in the form of a vector of "Instructions"
*/

#include "tokenizer.hpp"

#include <iostream>
#include <vector>
#include <string> // For std::stoi
#include <algorithm> // For std::rotate

enum InstructionTypes
{
	NORMAL,
	SHORT,
	LONG,
	JUMP
};

enum AddressingModes
{
	REGISTER,
	IMMIDIETE,
	DIRECT,
	INDIRECT
};

struct Instruction // Returns a vector of these
{
	int instructionType = 0;
	int opcode = 0;
	int addressingMode = 0;
	int switchData = 0;
	int data = 0;
};

struct JumpPoint
{
	std::string name;
	int address;
};

class Parser
{
public:
	std::vector<Instruction> parse(std::vector<Token> tokens, int offset)
	{
		if (!tokens.size())
			std::cout << "(Parser) Empty input" << std::endl;

		std::vector<Instruction> instructions;
		int index = 0;
		int currentAddress = 0;
		bool error = false;

		Instruction currentInstruction;

		// Will go through all the tokens, and apply the syntax rules, and will throw an error if syntax is incorrect.
		while (index < tokens.size())
		{
			// First token has to be a string
			if (tokens[index].type != TokenTypes::STRING)
			{
				std::cout << "(Parser) Expected string at index " << index << std::endl;
				return instructions;
			}

			int r = isPresent(&RESERVED_WORDS, tokens[index].value);
			if (r != -1)
			{
				switch (r)
				{
					case 3: case 6: case 15: // Short instruction

						currentInstruction.opcode = r;
						currentInstruction.instructionType = InstructionTypes::SHORT;

						currentAddress += 1;

					break;
					case 0: case 1: // Long instruction

						currentInstruction.opcode = r;
						currentInstruction.instructionType = InstructionTypes::LONG;

						if (index + 3 >= tokens.size()) break;

						// This is ugly, but it does the job. it basically checks if the syntax is correct and writes the appropriate data to the current instruction.
						// This process differs slightly between the different types of instructions, since more or less data has to be provided, but the concept stays the same

						if (tokens[++index].type == TokenTypes::NUMBER) { currentInstruction.switchData = std::stoi(tokens[index].value) & 0b11; } else { error = true; break; };
						if (tokens[++index].type == TokenTypes::SYMBOL) { currentInstruction.addressingMode = isPresent(&ADDRESSING_MODE_SYMBOLS, tokens[index].value); } else { error = true; break; };
						if (tokens[++index].type == TokenTypes::NUMBER) { currentInstruction.data = parseData(currentInstruction.addressingMode, tokens[index].value); } else { error = true; break; };

						currentAddress += (currentInstruction.addressingMode < 2) ? 2 : 3;

					break;
					case 4: case 5: // Jump instruction

						currentInstruction.opcode = r;
						currentInstruction.instructionType = InstructionTypes::JUMP;

						if (index + 2 >= tokens.size()) break;

						if (tokens[++index].type == TokenTypes::NUMBER) { currentInstruction.switchData = std::stoi(tokens[index].value) & 0b11; } else { error = true; break; };
						if (tokens[++index].type == TokenTypes::SYMBOL) { currentInstruction.addressingMode = isPresent(&ADDRESSING_MODE_SYMBOLS, tokens[index].value); } else { error = true; break; };
						if (tokens[++index].type != TokenTypes::SYMBOL)
						{
							// Look for jump point (basically an address) to jump to.
							if (tokens[index].type == TokenTypes::STRING)
							{
								for (int i = 0; i < jumpPoints.size(); i++)
								{
									if (jumpPoints[i].name == tokens[index].value)
									{
										currentInstruction.data = jumpPoints[i].address;
									}
								}
							}
							else
								currentInstruction.data = parseData(currentInstruction.addressingMode, tokens[index].value);
						}
						else error = true;

						currentAddress += 3;

					break;
					default: // Normal instruction

						currentInstruction.opcode = r;
						currentInstruction.instructionType = InstructionTypes::NORMAL;

						if (index + 2 >= tokens.size()) break;

						if (tokens[++index].type == TokenTypes::SYMBOL) { currentInstruction.addressingMode = isPresent(&ADDRESSING_MODE_SYMBOLS, tokens[index].value); } else { error = true; break; };
						if (tokens[++index].type == TokenTypes::NUMBER) { currentInstruction.data = parseData(currentInstruction.addressingMode, tokens[index].value); } else { error = true; break; };

						currentAddress += (currentInstruction.addressingMode < 2) ? 2 : 3;

					break;
				}

				if (error)
				{
					std::cout << "(Parser) Syntax error at index " << index << std::endl;
					return instructions;
				}

				instructions.push_back(currentInstruction);
			}
			else
			{
				// this adds a jump point, which can later be used when jumping. this is used so you dont have to remember a bunch of memory addresses.

				if (index + 1 >= tokens.size()) break;

				if (tokens[++index].value == ":")
				{
					JumpPoint jp;
					jp.address = currentAddress + offset;
					jp.name = tokens[index - 1].value;

					jumpPoints.push_back(jp);
				}
				else
				{
					std::cout << "(Parser) Expected : at index " << index << std::endl;
					return instructions;
				}
			}

			index++;
		}

		// displayes jump points
		std::cout << "\n|>---<| Defined jump points |>---<|\n";
		for (int i = 0; i < jumpPoints.size(); i++)
		{
			std::cout << jumpPoints[i].name << " : " << jumpPoints[i].address << std::endl;
		}
		std::cout << "|>-------------------------------<|\n\n";

		return instructions;
	}

private:
	// Lists of reserved words what are used to identify instructions or addressing modes.
	std::vector<std::string> RESERVED_WORDS = { "mov", "sto", "sps", "spl", "jmp", "sbr", "ret", "add", "sub", "and", "ior", "xor", "shl", "shr", "cmp", "hlt" };
	std::vector<std::string> ADDRESSING_MODE_SYMBOLS = { "&", "#", "$", "%" };

	std::vector<JumpPoint> jumpPoints;

	// Checks if element is present in provided list
	int isPresent(std::vector<std::string> *list, std::string value)
	{
		for (int i = 0; i < list->size(); i++)
		{
			if (list->at(i) == value)
				return i;
		}

		return -1;
	}

	// Parses data. Basically shaves off data to ensure that numbers aren't too big, according to the addressing mode.
	int parseData(int addressingMode, std::string number)
	{
		int r;

		switch (addressingMode)
		{
			default:
			case AddressingModes::REGISTER:
			case AddressingModes::IMMIDIETE:

				r = std::stoi(number) & 0xFF;

			break;

			case AddressingModes::DIRECT:
			case AddressingModes::INDIRECT:

				r = std::stoi(number) & 0xFFFF;

			break;
		}

		return r;
	}
};