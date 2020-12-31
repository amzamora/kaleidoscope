#include <iostream>
#include <fstream>
#include <sstream>

#include "types.hpp"
#include "lexer.hpp"

int main(int argc, char *argv[]) {
	//  Parse arguments
	if (argc != 2) {
		std::cout << "Usage: ./kaleidoscope file_path" << '\n' << '\n';
		throw std::exception();
	}

	// Read file
	std::ifstream in;
	in.open(argv[1]);
	std::stringstream stream;
	stream << in.rdbuf();
	std::string source_file = stream.str();

	// Lex file
	std::vector<Token> tokens = lex(source_file);

	for (auto it = tokens.begin(); it != tokens.end(); it++) {
		std::cout << (*it).literal << '\n';
	}

	return 0;
}