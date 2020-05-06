#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "Hyper16 Assembler" << std::endl;
	
	/*
	if (argc <= 1)
	{
		std::cout << "are u dumb" << std::endl;
		return -1;
	}
	*/

	std::string lol = "\n#A";

	std::cout << (int)lol.c_str()[0] << std::endl;

	return 0;
}